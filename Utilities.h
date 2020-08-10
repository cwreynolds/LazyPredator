//
//  Utilities.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/7/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>
#include <chrono>

// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) { grabPrintLock(); debugPrintNL(e); }

// Original (circa 2002) "non locking" version, in case it is ever useful.
#define debugPrintNL(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

// Use global mutex to allow synchronizing console output from parallel threads.
// (Written as a macro since the lock_guard is released at the end of a block.)
#define grabPrintLock() std::lock_guard<std::recursive_mutex> pl_(printMutex());
std::recursive_mutex& printMutex();

// Simple tool for inline timing sections of code. For example:
//    void foo()
//    {
//        Timer foo_timer("foo");
//        bar();
//        zap();
//    }
// After the block containing the Timer construction it prints:
//    foo elapsed time: 1.86984 seconds
// Added parameter to customize " elapsed time: " to any string including none:
//        Timer timer("Run time for unit test suite: ", "");
// prints:
//    Run time for unit test suite: 0.0301787 seconds
//
// TODO: perhaps customize message with a message_printing function pointer?
//
class Timer
{
public:
    Timer(const std::string& description)
      : description_(description),
    elasped_time_(" elapsed time: "),
    start_time_(std::chrono::high_resolution_clock::now()) {}
    Timer(const std::string& description, const std::string& elasped_time)
      : description_(description),
    elasped_time_(elasped_time),
    start_time_(std::chrono::high_resolution_clock::now()) {}
    ~Timer()
    {
        auto finish_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish_time - start_time_;
        std::cout << description_ << elasped_time_ << elapsed.count();
        std::cout << " seconds" << std::endl;
    }
private:
    const std::string description_;
    const std::string elasped_time_;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};
