//
// Created by  Oleksiy Grechnyev on 2/24/2017.
//

#ifndef CPPTHREADS_EVENT_H
#define CPPTHREADS_EVENT_H

namespace cppthreads {

    /// An arbitrary event with a timestamp and double data
    struct Event {
        /// Timestamp in ms since 1970
        long long timeStamp;

        /// Some data
        double beauty;

        /// Default constructor
        Event() {}

        /// Data constructor
        Event(long long int timeStamp, double beauty) : timeStamp(timeStamp), beauty(beauty) {}
    };
}

#endif //CPPTHREADS_EVENT_H
