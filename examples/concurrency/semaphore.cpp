// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/concurrency/semaphore.hpp>
#include <shard/random.hpp>

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

using namespace std::literals;

constexpr std::size_t max_threads = 8u;        // change and see the effect
constexpr std::ptrdiff_t max_sema_threads = 2; // 1 for binary semaphore
shard::semaphore g_semaphore(max_sema_threads);
constexpr auto time_tick = 10ms;

static shard::random<unsigned int>::value_type rnd() {
    static shard::random<unsigned int> rng(2, 9);
    rng.reseed();
    return rng.next();
}

class alignas(128 /* std::hardware_destructive_interference_size */) job {
public:
    static void start_time() { s_started_at = std::chrono::high_resolution_clock::now(); }

    void initial_delay() const { std::this_thread::sleep_for(m_delay * time_tick); }

    void execute() {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_started_at - m_delay * time_tick);
        m_wait_time = static_cast<unsigned>(duration.count() / time_tick.count());
        std::this_thread::sleep_for(m_work_time * time_tick);
    }

    void visualize(unsigned id, unsigned int x_scale = 2) const {
        auto print_n = [=](const char* str, unsigned int n) {
            n *= x_scale;
            while (n-- > 0) {
                std::cout << str;
            }
        };
        std::lock_guard lock(s_cout_mutex);
        std::cout << "#" << std::setw(3) << id << " ";
        print_n("░", m_delay);
        print_n("▒", m_wait_time);
        print_n("█", m_work_time);
        std::cout << '\n';
    }

    static void show_info() {
        std::cout << "threads: " << max_threads << ", throughput: " << max_sema_threads << '\n';
        std::cout << "legend: delayed ░░ | waiting ▒▒ | working ██\n";
        std::cout << std::endl;
    }

private:
    inline static std::mutex s_cout_mutex;
    inline static std::chrono::high_resolution_clock::time_point s_started_at;
    unsigned int m_delay = rnd();
    unsigned int m_work_time = rnd();
    unsigned int m_wait_time = 0;
};

std::array<job, max_threads> g_jobs;

void worker_thread(unsigned id) {
    g_jobs[id].initial_delay(); // emulate some work before sema acquisition
    g_semaphore.acquire();      // wait until a free sema slot is available
    g_jobs[id].execute();       // emulate some work while sema is acquired
    g_semaphore.release();      // release the semaphore
    g_jobs[id].visualize(id);   // visualize the time spent in each state
}

int main() {
    std::vector<std::thread> threads;
    threads.reserve(max_threads);

    job::show_info();
    job::start_time();

    for (auto id = 0u; id != max_threads; ++id) {
        threads.emplace_back(worker_thread, id);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
