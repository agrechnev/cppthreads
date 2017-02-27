cmake_minimum_required(VERSION 3.0)
project(cppthreads)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RingBuffer.h ReadEngine.cpp ReadEngine.h Event.h)
add_executable(cppthreads ${SOURCE_FILES})