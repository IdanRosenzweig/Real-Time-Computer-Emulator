#ifndef EMULATOR_COMMANDS_QUEUE_H
#define EMULATOR_COMMANDS_QUEUE_H

#include <stdint.h>
#include <queue>
#include <mutex>

struct commands_queue {
    std::queue <uint8_t> queue;
    std::mutex mtx;
    uint16_t max;

    void enqueue(uint8_t v) {
        while (max && queue.size() >= max)
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        mtx.lock();

        queue.push(v);

        mtx.unlock();
    }

    uint8_t dequeue() {
        mtx.lock();

        while (queue.empty()) {
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(50));
            mtx.lock();
        }

        uint8_t v = queue.front();
        queue.pop();

        mtx.unlock();

        return v;
    }
};


#endif
