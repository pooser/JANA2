//
// Created by nbrei on 4/4/19.
//

#include <JANA/JWorker.h>


JWorker::Metrics::Metrics()
        : _useful_time(duration_t::zero())
        , _retry_time(duration_t::zero())
        , _scheduler_time(duration_t::zero())
        , _idle_time(duration_t::zero())
        , _scheduler_visits(0) {}


void JWorker::Metrics::update(const JWorker::Metrics& other) {

    _mutex.lock();
    _useful_time += other._useful_time;
    _retry_time += other._retry_time;
    _scheduler_time += other._scheduler_time;
    _idle_time += other._idle_time;
    _scheduler_visits += other._scheduler_visits;
    _mutex.unlock();
}


void JWorker::Metrics::update(const duration_t& useful_time,
                             const duration_t& retry_time,
                             const duration_t& scheduler_time,
                             const duration_t& idle_time,
                             const long& scheduler_visits) {
    _mutex.lock();
    _useful_time += useful_time;
    _retry_time += retry_time;
    _scheduler_time += scheduler_time;
    _idle_time = idle_time;
    _scheduler_visits += scheduler_visits;
    _mutex.unlock();
}


void JWorker::Metrics::get(duration_t& useful_time,
                          duration_t& retry_time,
                          duration_t& scheduler_time,
                          duration_t& idle_time,
                          long& scheduler_visits) {
    _mutex.lock();
    useful_time = _useful_time;
    retry_time = _retry_time;
    scheduler_time = _scheduler_time;
    idle_time = _idle_time;
    scheduler_visits = _scheduler_visits;
    _mutex.unlock();
}


JWorker::Summary JWorker::get_summary() {

    using millis = std::chrono::duration<double, std::milli>;
    JWorker::Summary summary;
    summary.worker_id = worker_id;
    summary.last_arrow_name = ((assignment == nullptr) ? "idle" : assignment->get_name());

    duration_t useful_time, retry_time, scheduler_time, idle_time;
    long scheduler_visits;
    metrics.get(useful_time, retry_time, scheduler_time, idle_time, scheduler_visits);
    duration_t total_time = useful_time + retry_time + scheduler_time + idle_time;

    summary.useful_time_frac = millis(useful_time)/millis(total_time);
    summary.retry_time_frac = millis(retry_time)/millis(total_time);
    summary.idle_time_frac = millis(idle_time)/millis(total_time);
    summary.scheduler_time_frac = millis(scheduler_time)/millis(total_time);
    summary.scheduler_visits = scheduler_visits;
    return summary;
}


JWorker::JWorker(uint32_t id, JScheduler& scheduler) :
        _scheduler(scheduler), worker_id(id), assignment(nullptr) {

    _thread = new std::thread(&JWorker::loop, this);
    if (serviceLocator != nullptr) {
        LOG_DEBUG(_logger) << "JWorker " << worker_id << " found parameters from serviceLocator." << LOG_END;
        auto params = serviceLocator->get<ParameterManager>();
        checkin_time = params->checkin_time;
        initial_backoff_time = params->backoff_time;
        backoff_tries = params->backoff_tries;
    }
    _logger = JLoggingService::logger("JWorker");
    LOG_DEBUG(_logger) << "JWorker " << worker_id << " constructed." << LOG_END;
}


JWorker::~JWorker() {
    /// We have to be careful here because this Worker might be being concurrently
    /// read/modified by Worker.thread. Join with thread before doing anything else.

    LOG_DEBUG(_logger) << "JWorker " << worker_id << " destruction has begun." << LOG_END;
    if (_thread == nullptr) {
        LOG_ERROR(_logger) << "JWorker " << worker_id << " thread is null. This means we deleted twice somehow!"
                           << LOG_END;
    } else {
        _thread->join();            // Does this throw? Can we guarantee this terminates?
        delete _thread;             // Should JWorker lifetime really match thread lifetime?
        _thread = nullptr;          // Catch and log error if somehow we try to delete twice
    }
    LOG_DEBUG(_logger) << "JWorker " << worker_id << " destruction has completed." << LOG_END;
}


void JWorker::loop() {

    LOG_DEBUG(_logger) << "JWorker " << worker_id << " has entered loop()." << LOG_END;
    JArrow::Status last_result = JArrow::Status::ComeBackLater;

    while (!shutdown_requested) {

        LOG_DEBUG(_logger) << "JWorker " << worker_id << " is checking in" << LOG_END;
        auto start_time = jclock_t::now();

        assignment = _scheduler.next_assignment(worker_id, assignment, last_result);
        last_result = JArrow::Status::KeepGoing;

        auto scheduler_time = jclock_t::now();

        auto scheduler_duration = scheduler_time - start_time;
        auto idle_duration = duration_t::zero();
        auto retry_duration = duration_t::zero();
        auto useful_duration = duration_t::zero();

        if (assignment == nullptr) {
            LOG_DEBUG(_logger) << "JWorker " << worker_id
                               << " idling due to lack of assignments" << LOG_END;
            std::this_thread::sleep_for(checkin_time);
            idle_duration = jclock_t::now() - scheduler_time;
        }
        else {

            uint32_t current_tries = 0;
            auto backoff_duration = initial_backoff_time;

            while (current_tries <= backoff_tries &&
                   (last_result == JArrow::Status::KeepGoing || last_result == JArrow::Status::ComeBackLater) &&
                   !shutdown_requested &&
                   (jclock_t::now() - start_time) < checkin_time) {

                LOG_DEBUG(_logger) << "JWorker " << worker_id << " is executing "
                                   << assignment->get_name() << LOG_END;
                auto before_execute_time = jclock_t::now();
                last_result = assignment->execute();
                useful_duration += (jclock_t::now() - before_execute_time);


                if (last_result == JArrow::Status::KeepGoing) {
                    LOG_DEBUG(_logger) << "JWorker " << worker_id << " succeeded at "
                                       << assignment->get_name() << LOG_END;
                    current_tries = 0;
                    backoff_duration = initial_backoff_time;
                }
                else {
                    current_tries++;
                    if (backoff_tries > 0) {
                        if (backoff_strategy == BackoffStrategy::Linear) {
                            backoff_duration += initial_backoff_time;
                        }
                        else if (backoff_strategy == BackoffStrategy::Exponential) {
                            backoff_duration *= 2;
                        }
                        LOG_DEBUG(_logger) << "JWorker " << worker_id << " backing off with "
                                           << assignment->get_name() << ", tries = " << current_tries
                                           << LOG_END;

                        std::this_thread::sleep_for(backoff_duration);
                        retry_duration += backoff_duration;
                        // TODO: Randomize backoff duration?
                    }
                }
            }
        }
        metrics.update(useful_duration, retry_duration, scheduler_duration, idle_duration, 1);
    }

    _scheduler.last_assignment(worker_id, assignment, last_result);

    LOG_DEBUG(_logger) << "JWorker " << worker_id << " is exiting loop()" << LOG_END;
    shutdown_achieved = true;
}



