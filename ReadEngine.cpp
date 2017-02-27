//
// Created by  Oleksiy Grechnyev on 2/24/2017.
//

#include "ReadEngine.h"


#include <chrono>

namespace cppthreads {

    ReadEngine::ReadEngine(RingBuffer<Event> &buffer) : buffer(buffer) {
        using namespace std;


        // Start the thread. Lambda captures fields of this via & (refs).
        readThread = thread([&](){
            cout << "Reading thread : START" << endl;

            while (!willStop) {
                Event e;

                if (!buffer.isEmpty()) {
                    if (buffer.pop(e)) {
                        cout << e.timeStamp << " : " << e.beauty << endl;
                    }
                } else {
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }

            cout << "Reading thread : STOP" << endl;

        });
    }
}