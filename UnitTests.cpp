//
//  UnitTests.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"
#include "TexSynTemp.h"

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

bool random_program_size_limit()
{
    bool all_ok = true;
    int total_subtests = 1000;
    RandomSequence rs(77365918);
    FunctionSet& fs = TexSynFS::full();
    for (int i = 0; i < total_subtests; i++)
    {
        int max_size = int(rs.frandom2(4, 100));
        int actual_size = 0;
        std::string source_code;
        fs.dp_depth = 0;
        GpTree gp_tree;
        fs.makeRandomProgram(max_size, "Texture",
                             actual_size, source_code, gp_tree);
        if (!st(gp_tree.size() <= max_size)) all_ok = false;
        if (!st(actual_size <= max_size)) all_ok = false;
    }
    return all_ok;
}

bool test_gp_tree_construction()
{
    GpTree root;
    bool created_empty = st(root.subtrees().empty());
    root.addSubtrees(2);                // add r.0 and r.1
    root.getSubtree(1).addSubtrees(1);  // add r.1.a

    root.setId("r");
    root.getSubtree(0).setId("r.0");
    root.getSubtree(1).setId("r.1");
    root.getSubtree(1).getSubtree(0).setId("r.1.a");

    return (created_empty &&
            st(root.subtrees().size() == 2) &&
            st(root.id() == "r") &&
            st(root.getSubtree(0).id() == "r.0") &&
            st(root.getSubtree(1).id() == "r.1") &&
            st(root.getSubtree(1).getSubtree(0).id() == "r.1.a") &&
            true);
}

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    
    logAndTally(mock);
    logAndTally(population_allocation_of_individuals);
    logAndTally(random_program_size_limit);
    logAndTally(test_gp_tree_construction);

    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}
