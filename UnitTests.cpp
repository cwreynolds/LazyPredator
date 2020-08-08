//
//  UnitTests.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"

// This "sub-test" wrapper macro just returns the value of the given expression
// "e". If the value is NOT TRUE, the st() macro will also log the specific
// failing sub-test expression ("e") to aid in debugging. Unit test functions
// generally run several sub-tests ANDing the results together.
#define st(e) [&]()                                        \
{                                                          \
    bool _e_ok = (e);                                      \
    if (!_e_ok) std::cout << "fail: " << #e << std::endl;  \
    return _e_ok;                                          \
}()

// Used only in UnitTests::allTestsOK()
#define logAndTally(e)                       \
{                                            \
bool _e_ok = e();                        \
std::cout << "\t";                       \
std::cout << (_e_ok ? "pass" : "FAIL");  \
std::cout << " " << #e;                  \
std::cout << std::endl << std::flush;    \
if (!_e_ok) all_tests_passed = false;    \
}

bool mock() { return true; }

bool population_allocation_of_individuals()
{
    bool start_with_none = st(Individual::getInstanceCount() == 0);
    int target_count = 3;
    bool match_target_count = false;
    // These brackets serve to delimit the lifetime of Population "p".
    {
        Population p(target_count);
        match_target_count = st(Individual::getInstanceCount() == target_count);
    }
    bool end_with_none = st(Individual::getInstanceCount() == 0);
    return start_with_none && match_target_count && end_with_none;
}

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    
    logAndTally(mock);
    logAndTally(population_allocation_of_individuals);
    
    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}
