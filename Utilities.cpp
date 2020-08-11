//
//  Utilities.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/7/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "Utilities.h"

// TODO UTILITIES_NAMESPACES

// Global mutex to allow synchronizing console output from parallel threads.
std::recursive_mutex print_mutex_;
// Acessor.
std::recursive_mutex& printMutex() { return print_mutex_; }

// Takes a 32 bit value and shuffles it around to produce a new 32 bit value.
//
// "Robert Jenkins' 32 bit integer hash function" from "Integer Hash Function"
// (1997) by Thomas Wang (https://gist.github.com/badboy/6267743)
// Altered to accept input as uint64_t but ignores the top 32 bits.
uint32_t rehash32bits(uint64_t u64)
{
    uint32_t a = uint32_t(u64);
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}
