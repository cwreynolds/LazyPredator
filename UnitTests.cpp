//
//  UnitTests.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"
#include "TestFS.h"

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
    // Ideally this should be just a reference to FS in TestFS. Copy for now.
    FunctionSet fs = TestFS::testTreeEval();
    for (int i = 0; i < total_subtests; i++)
    {
        int max_size = int(rs.frandom2(4, 100));
        int actual_size = 0;
        std::string source_code;
        fs.dp_depth = 0;
        GpTree gp_tree;
        fs.makeRandomProgram(max_size, "Float",
                             actual_size, source_code, gp_tree);
        bool ok = (st(actual_size <= max_size) &&
                   st(gp_tree.size() <= max_size)) &&
                   st(actual_size == gp_tree.size()) &&
                   st(source_code == gp_tree.to_string());
        if (!ok) all_ok = false;
    }
    return all_ok;
}

bool test_gp_tree_construction()
{
    GpTree root;                                      // make empty tree
    bool created_empty = st(root.subtrees().empty()); // verify empty
    root.addSubtrees(2);                              // add 2 subtrees under r
    GpTree& st0 = root.getSubtree(0);                 // name for subtree r.0
    GpTree& st1 = root.getSubtree(1);                 // name for subtree r.1
    st1.addSubtrees(1);                               // add 1 subtree under r.1
    GpTree& st10 = root.getSubtree(1).getSubtree(0);  // name for subtree r.1.a
    // Set leaf values to names as character string.
    root.setLeafValue(std::string("r"));
    st0.setLeafValue(std::string("r.0"));
    st1.setLeafValue(std::string("r.1"));
    st10.setLeafValue(std::string("r.1.a"));
    // Check for tree depth, breath, and labels.
    return (created_empty &&
            st(root.subtrees().size() == 2) &&
            st(st0.subtrees().size() == 0) &&
            st(st1.subtrees().size() == 1) &&
            st(st10.subtrees().size() == 0) &&
            st(std::any_cast<std::string>(root.getLeafValue()) == "r") &&
            st(std::any_cast<std::string>(st0.getLeafValue())  == "r.0") &&
            st(std::any_cast<std::string>(st1.getLeafValue())  == "r.1") &&
            st(std::any_cast<std::string>(st10.getLeafValue()) == "r.1.a"));
}

bool test_gp_tree_eval_pod()  // For simple case of "plain old data" types.
{
    FunctionSet fs = TestFS::testTreeEval();
    
    // Construct a tree for "AddInt(1, Floor(2.5))"
    int leaf1 = 1;                        // Leaf value Int 1
    float leaf25 = 2.5;                   // Leaf value Float 2.5
    int expected = leaf1 + int(leaf25);   // Expected return value Int 3
    GpType& gp_type_int = *fs.lookupGpTypeByName("Int");
    // GpType& gp_type_float = *fs.lookupGpTypeByName("Float");
    GpFunction& gp_func_addint = *fs.lookupGpFunctionByName("AddInt");
    GpFunction& gp_func_floor = *fs.lookupGpFunctionByName("Floor");
    GpTree gp_tree;                       // Make empty tree.
    gp_tree.setFunction(gp_func_addint);  // Set root function to AddInt.
    gp_tree.setType(gp_type_int);         // Set root type to Int.
    gp_tree.addSubtrees(2);               // AddInt has two parameters.
    
    GpTree& st0 = gp_tree.getSubtree(0);  // Name for substree 0.
    GpTree& st1 = gp_tree.getSubtree(1);  // Name for substree 1.
    st1.addSubtrees(1);                   // "Floor" has one parameter.
    GpTree& st10 = st1.getSubtree(0);     // Name for substree 0 of st1.
    st0.setLeafValue(std::any(leaf1));    // Subtree 0 is leaf const Int 1.
    st10.setLeafValue(std::any(leaf25));  // Subtree 1,0 is leaf Float 2.5.
    st1.setFunction(gp_func_floor);       // Subtree 1 has function Floor.
    // TODO when this incorrectly said gp_type_float, eval still worked
    //      Noticed when I added st below to check st1.getType().
    //      Should verify they match, or set both in setFunction().
    st1.setType(gp_type_int);             // Subtree 1 has type Float.

    // TODO FWIW, the first four subtests duplicate test_gp_tree_construction().
    return (st(gp_tree.subtrees().size() == 2) &&
            st(st0.subtrees().size() == 0) &&
            st(st1.subtrees().size() == 1) &&
            st(st10.subtrees().size() == 0) &&
            st(&gp_tree.getType() == &gp_type_int) &&
            st(&gp_tree.getFunction() == &gp_func_addint) &&
            st(&st1.getType() == &gp_type_int) &&
            st(&st1.getFunction() == &gp_func_floor) &&
            st(std::any_cast<int>(gp_tree.eval()) == expected));
}

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    
    logAndTally(population_allocation_of_individuals);
    logAndTally(random_program_size_limit);
    logAndTally(test_gp_tree_construction);
    logAndTally(test_gp_tree_eval_pod);

    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}
