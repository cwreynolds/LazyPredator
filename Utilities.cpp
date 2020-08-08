//
//  Utilities.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/7/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "Utilities.h"

// Global mutex to allow synchronizing console output from parallel threads.
std::recursive_mutex print_mutex_;
// Acessor.
std::recursive_mutex& printMutex() { return print_mutex_; }
