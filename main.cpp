#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <random>
#include <chrono>
#include <ctime>

#include "RingBuffer.h"
#include "Event.h"
#include "ReadEngine.h"


int main() {
    using namespace std;
    using namespace cppthreads;

    // Random number generator

    // Note: random_device is not random in MinGW !
//    random_device rd;
//    mt19937 mt(rd());

    // Seed with time the old-fashioned way
    mt19937 randomEngine(time(nullptr));

    // The buffer
    RingBuffer<Event> buffer(5);

    // Create the read engine and start the thread
    ReadEngine readEngine(buffer);

    this_thread::sleep_for(chrono::milliseconds(200));


    // Create a few data-sending threads
    constexpr int NTHREAD = 10;
    thread threads[NTHREAD];
    for (int i = 0; i < NTHREAD; i++) {

        // Use captured REF to buffer and randomEngine, important
        threads[i] = thread([&buffer, &randomEngine](int ind) {
            // Random uniform distribution from 0 to 1
            uniform_real_distribution<double> uDouble01(0.0, 1.0);

            for (int j = 0; j < 2; ++j) {

                // Push something into a bufffer
                // Timestamp since 1970
                long long timeStamp = chrono::duration_cast< chrono::milliseconds >(
                        chrono::system_clock::now().time_since_epoch()
                ).count();

                // Integer part = ind (thread number) + random fractional part
                double beauty = (double)ind + uDouble01(randomEngine);
                buffer.push(Event(timeStamp, beauty));

                // Take a nap
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }, i); // END lambda

        this_thread::sleep_for(chrono::milliseconds(50));
    }

    // Wait for the write threads to finish
    for (auto &t : threads) {
        t.join();
    }


    this_thread::sleep_for(chrono::seconds(5));

    // Stop the write engine (it doesn't quit by itself, so no join)
    readEngine.stop();


    return 0;
}