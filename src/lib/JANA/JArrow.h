//
// Created by nbrei on 3/25/19.
//

#ifndef GREENFIELD_ARROW_H
#define GREENFIELD_ARROW_H

#include <vector>

#include <JANA/Queue.h>
#include "JServiceLocator.h"



using duration_t = std::chrono::steady_clock::duration;

class JArrow : public JActivable {


private:
    // Constants
    const std::string _name;     // Used for human understanding
    const bool _is_parallel;     // Whether or not it is safe to parallelize

    // Written internally, read externally
    size_t _message_count = 0;   // Total number of messages completed by this arrow
    size_t _queue_visits = 0;    // Total number of times execute() calls inqueue.pop()
    duration_t _total_latency;   // Total time spent doing actual work (across all cpus)
    duration_t _last_latency;    // Most recent latency measurement (from a single cpu)
    duration_t _queue_overhead;  // Total time spent pushing and popping from queues

    // Written externally
    size_t _chunksize = 1;       // Number of items to pop off the input queue at once
    size_t _thread_count = 0;    // Current number of threads assigned to this arrow
    bool _is_upstream_finished = false;

    std::mutex _mutex;           // Protects access to arrow properties.
                                 // TODO: Consider storing and protect thread count differently,
                                 // so that (number of workers) = (sum of thread counts for all arrows)
                                 // This is not so simple if we also want our WorkerStatus::arrow_name to match


public:

    /// Status enum for communicating
    enum class Status {KeepGoing, ComeBackLater, Finished, Error};

    // Constants

    bool is_parallel() { return _is_parallel; }

    std::string get_name() { return _name; }



    // Written internally, read externally

    bool is_upstream_finished() { return _is_upstream_finished; }


    void get_metrics(size_t& message_count,
                     size_t& queue_visits,
                     duration_t& total_latency,
                     duration_t& queue_overhead,
                     duration_t& last_latency) {

        std::lock_guard<std::mutex> lock(_mutex);
        message_count = _message_count;
        queue_visits = _queue_visits;
        total_latency = _total_latency;
        queue_overhead = _queue_overhead;
        last_latency = _last_latency;
    }



protected:

    // Written internally, read externally

    void set_upstream_finished(bool upstream_finished) { _is_upstream_finished = upstream_finished; }


    void update_metrics(size_t message_count_delta,
                        size_t queue_visits_delta,
                        duration_t latency,
                        duration_t queue_overhead) {

        std::lock_guard<std::mutex> lock(_mutex);

        _queue_visits += queue_visits_delta;
        _queue_overhead += queue_overhead;

        if (message_count_delta != 0) {
            _message_count += message_count_delta;
            _total_latency += latency;
            _last_latency = latency/message_count_delta;
        }
    }


public:

    // Written externally

    void set_chunksize(size_t chunksize) {
        std::lock_guard<std::mutex> lock(_mutex);
        _chunksize = chunksize;
    }

    size_t get_chunksize() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _chunksize;
    }

    void update_thread_count(size_t thread_count_delta) {
        std::lock_guard<std::mutex> lock(_mutex);
        _thread_count += thread_count_delta;
    }

    size_t get_thread_count() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _thread_count;
    }


    JArrow(std::string name, bool is_parallel) :
            _name(std::move(name)),
            _is_parallel(is_parallel),
            _total_latency(duration_t::zero()),
            _last_latency(duration_t::zero()),
            _queue_overhead(duration_t::zero()) {

        if (serviceLocator != nullptr) {
            auto params = serviceLocator->get<ParameterManager>();
            _chunksize = params->chunksize;
        }
    };

    virtual ~JArrow() = default;

    virtual Status execute() = 0;

};

inline std::string to_string(JArrow::Status h) {
    switch (h) {
        case JArrow::Status::KeepGoing:     return "KeepGoing";
        case JArrow::Status::ComeBackLater: return "ComeBackLater";
        case JArrow::Status::Finished:      return "Finished";
        case JArrow::Status::Error:
        default:                          return "Error";
    }
}




#endif // GREENFIELD_ARROW_H