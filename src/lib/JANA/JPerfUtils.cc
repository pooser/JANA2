//
// Created by nbrei on 3/28/19.
//

#include <thread>
#include <algorithm>

#include "JPerfUtils.h"


thread_local std::mt19937* generator = nullptr;

uint64_t consume_cpu_ms(uint64_t millisecs, double spread) {

    uint64_t sampled = rand_size(millisecs, spread);
    auto duration = std::chrono::milliseconds(sampled);

    auto start_time = std::chrono::steady_clock::now();
    uint64_t result = 0;

    while ((std::chrono::steady_clock::now() - start_time) < duration) {

        double a = (*generator)();
        double b = sqrt(a * pow(1.23, -a)) / a;
        result += long(b);
    }
    return result;
}

uint64_t read_memory(const std::vector<char>& buffer) {

    auto length = buffer.size();
    uint64_t sum = 0;
    for (unsigned i=0; i<length; ++i) {
        sum += buffer[i];
    }
    return sum;
}

uint64_t write_memory(std::vector<char>& buffer, uint64_t bytes, double spread) {

    uint64_t sampled = rand_size(bytes, spread);
    for (unsigned i=0; i<sampled; ++i) {
        buffer.push_back(2);
    }
    return sampled*2;
}

size_t rand_size(size_t avg, double spread) {

    auto delta = static_cast<size_t>(avg*spread);

    if (!generator) {
        std::hash<std::thread::id> hasher;
        long now = std::chrono::steady_clock::now().time_since_epoch().count();
        long seed = now + hasher(std::this_thread::get_id());
        generator = new std::mt19937(seed);
    }
    std::uniform_int_distribution<size_t> distribution(avg-delta, avg+delta);
    return distribution(*generator);
}


int randint(int min, int max) {

    if (!generator) {
        long now = std::chrono::steady_clock::now().time_since_epoch().count();
        std::hash<std::thread::id> hasher;
        long seed = now + hasher(std::this_thread::get_id());
        generator = new std::mt19937(seed);
    }
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}

double randdouble() {
    return (*generator)();
}




