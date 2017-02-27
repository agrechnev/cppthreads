//
// Created by  Oleksiy Grechnyev on 2/24/2017.
//

#ifndef CPPTHREADS_RINGBUFFER_H
#define CPPTHREADS_RINGBUFFER_H

#include <mutex>
#include <cassert>

namespace cppthreads {

/** \brief A very simple Ring Buffer implementation
 *
 * push() and pop() are synchronized, the buffer is supposed to be thread-safe
 *
 * Note: It operates elements by value always.
 * If you want to keep refs instead, try  T = shared_ptr<someclass>
 *
 * @tparam T       Element type
 * @tparam size    Size of the bufer
 */
    template<class T>
    class RingBuffer {
    public:
        /// Constructor
        RingBuffer(int size) : size(size) {
            assert(size > 0);

            // The good old dynamic array
            data = new T[size];
        }

        /// Destructor
        ~RingBuffer(){
            delete [] data;
        }

        /// Is the buffer full?
        bool isFull() const{
            return  (readIndex == writeIndex) && !empty;
        }

        /// Is the buffer empty?
        bool isEmpty() const {
            return empty;
        }

        /** \brief Push an element into the list, synchronized
         *
         * @param[in]    t    element
         * @return            true is success, false if full buffer
         */
        bool push(T t){
            if (isFull()) return false;

            {
                // Synchronized block starts here
                std::lock_guard<std::mutex> guard(bufferLock);

                data[writeIndex] = t; // Write data
                next(writeIndex); // Increment write index by 1
                empty = false; // Clear the empty flag after write

                return true;
            }
        }

        /** \brief Pop an element from the list, synchronized
         *
         * @param[out] t  element
         * @return        true if success, false if empty buffer
         */
        bool pop(T & t) {
            // Check if empty
            if (empty) return false;

            {
                // Synchronized block starts here
                std::lock_guard<std::mutex> guard(bufferLock);

                t = data[readIndex]; // Read data
                next(readIndex);  // Increment read index by 1

                empty =  (readIndex == writeIndex); // Set the empty flag if empty

                return true;
            }
        }

    private:
        /// Disable copy constructor
        RingBuffer(const RingBuffer<T> & other) = delete;

        /// Disable assignment
        RingBuffer & operator=(const RingBuffer<T> &) = delete;

        /// Increment i by 1, wrapping around size
        void next(int & i){
            i++;
            i %= size;
        }

        /// Buffer size
        int size;

        /// The data as a dynamic array of size size
        T * data;

        // Note readIndex == writeIndex means either empty or full buffer

        /// Next position to read from
        int readIndex = 0;

        /// Next Position to write to
        int writeIndex = 0;

        /// Is the buffer empty ? (to distinguish between the empty and full cases)
        bool empty = true;

        /// Mutex used to synchronize the buffer
        std::mutex bufferLock;

    };

}

#endif //CPPTHREADS_RINGBUFFER_H
