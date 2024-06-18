
#include "pit.hpp"
#include "pit_input_codes.h"
#include "pit_output_codes.h"
#include "pit_access_modes.h"

#include <string.h>
#include <thread>

pit::pit() {
    for (int i = 0; i < NO_INTERNAL_TIMERS; i++) {
        memset(&timers[i], '\x00', sizeof(internal_timer_t));
        timers[i].count = timers[i].def = 0xffff;
    }
}

pit::~pit() {
    for (int i = 0; i < NO_INTERNAL_TIMERS; i++)
        if (timers[i].th.joinable()) {
            timers[i].running = false;
            timers[i].th.join();
        }
}


uint8_t pit::read_byte_from_port(uint16_t addr) {

    switch (addr) {
        case pit_input_codes::CHANNEL_0_DATA_PORT:
        case pit_input_codes::CHANNEL_1_DATA_PORT:
        case pit_input_codes::CHANNEL_2_DATA_PORT: {
            uint8_t rgn = addr & 0b11;

            switch (cwr.RL) { // switch on access mode
                case pit_access_mode::LOBYTE:
                lobyte:
                    return timers[rgn].count >> 8;

                case pit_access_mode::HIBYTE:
                hibyte:
                    return timers[rgn].count & 0xff;

                case pit_access_mode::LOBYTE_HIBYTE: {
                    if (!(r_lobyte ^= true)) // LOBYTE
                        goto lobyte;
//                        return timers[rgn].count >> 8;
                    else // HIBYTE
                        goto hibyte;
//                        return timers[rgn].count & 0xff;}
                }
            }
        }

        default:
            return 0;
    }
}

void pit::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        case pit_output_codes::CHANNEL_0_DATA_PORT:
        case pit_output_codes::CHANNEL_1_DATA_PORT:
        case pit_output_codes::CHANNEL_2_DATA_PORT: {
            uint8_t rgn = addr & 0x3;

            //if(cwr.SC != rgn)
            //	break;
            switch (cwr.RL) { // switch on access mode
                case pit_access_mode::LOBYTE:
                lobyte:
                    timers[rgn].count = (timers[rgn].count & 0xff00) + v;
                    break;

                case pit_access_mode::HIBYTE:
                hibyte:
                    timers[rgn].count = (v << 8) + (timers[rgn].count & 0xff);
                    break;

                case pit_access_mode::LOBYTE_HIBYTE:
                    if (!(r_lobyte ^= true)) // LOBYTE
                        goto lobyte;
                    else // HIBYTE
                        goto hibyte;
                    break;
            }

            timers[rgn].def = timers[rgn].count;

            break;
        }

        case pit_output_codes::MODE_COMMAND_REG:
            cwr.raw = v;

            if (cwr.SC < NO_INTERNAL_TIMERS) { // is the select refers to one of the timers
                timers[cwr.SC].mode = cwr.MODE;

                switch (cwr.RL) { // switch on access mode
                    case pit_access_mode::LATCH_COUNT_COMMAND:
                        timers[cwr.SC].def = timers[cwr.SC].count;
                        break;

                    case pit_access_mode::LOBYTE_HIBYTE:
                        r_lobyte = true;
                        break;
                }
            }

            if (!timers[cwr.SC].th.joinable()) { // timer is not running, start it
                timers[cwr.SC].running = true;
                timers[cwr.SC].th = std::thread(&pit::worker, this, &timers[cwr.SC]);
            }

            break;
    }
}


void pit::worker(internal_timer_t *timer) {
    while (timer->running) {
        switch (timer->mode) {
            case 2: {
                // need to sleep timer->def / FREQUENCY seconds
                long double milli = (long double) (1000 * timer->def) / (long double) (FREQUENCY);
                std::this_thread::sleep_for(std::chrono::milliseconds((int) milli));

                intr_req = true;
                break;
            }

            default: {
#define TIMER_EPSILON_MILLI 100
                // sleep the epsilon
                std::this_thread::sleep_for(std::chrono::milliseconds(TIMER_EPSILON_MILLI));
            }
        }
    }
}
