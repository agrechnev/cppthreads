//
// Created by  Oleksiy Grechnyev on 2/24/2017.
//

#ifndef CPPTHREADS_READENGINE_H
#define CPPTHREADS_READENGINE_H

#include <thread>
#include <iostream>

#include "RingBuffer.h"
#include "Event.h"

namespace cppthreads {
/** \brief Runs a thread that reads events from a ring buffer
 *
 */
    class ReadEngine {
    public:
        /// Constructor
        ReadEngine(RingBuffer<Event> &buffer);

        /// Destructor
        ~ReadEngine() {
            stop();
        }

        /// Stop the thread if not stopped already
        void stop() {
            if (!stopped) {
                willStop = true;
                readThread.join();
                stopped = true;
            }
        }

    private:
        /// The ring buffer to read events from
        RingBuffer<Event> &buffer;

        /// Boolean flag used to stop the thread
        bool willStop = false;

        /// Boolean flag to show if stopped already
        bool stopped = false;

        /// The reading thread object
        std::thread readThread;
    };
}


#endif //CPPTHREADS_READENGINE_H
