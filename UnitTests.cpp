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
    const FunctionSet& fs = TestFS::treeEval();
    for (int i = 0; i < total_subtests; i++)
    {
        int max_size = int(rs.frandom2(4, 100));
        int actual_size = 0;  // Output arg, set to actual size of random tree.
        GpTree gp_tree;       // Output arg, stores random tree.
        fs.makeRandomProgram(max_size, "Float", actual_size, gp_tree);
        // Are both measures of size (from makeRandomProgram() and measured
        // after the fact by GpTree) within given limit? And they match?
        bool ok = (st(actual_size <= max_size) &&
                   st(gp_tree.size() <= max_size) &&
                   st(actual_size == gp_tree.size()));
        if (!ok) all_ok = false;
    }
    return all_ok;
}

bool gp_tree_construction()
{
    GpTree root;                                      // make empty tree
    bool created_empty = st(root.subtrees().empty()); // verify empty
    root.addSubtrees(2);                              // add 2 subtrees under r
    GpTree& st0 = root.getSubtree(0);                 // name for subtree r.0
    GpTree& st1 = root.getSubtree(1);                 // name for subtree r.1
    st1.addSubtrees(1);                               // add 1 subtree under r.1
    GpTree& st10 = root.getSubtree(1).getSubtree(0);  // name for subtree r.1.a
    // Set leaf values to names as character string.
    GpType str("String");
    root.setLeafValue(std::string("r"), str);
    st0.setLeafValue(std::string("r.0"), str);
    st1.setLeafValue(std::string("r.1"), str);
    st10.setLeafValue(std::string("r.1.a"), str);
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

bool gp_tree_eval_simple()  // For simple case of "plain old data" types.
{
    // Construct a tree for "AddInt(1, Floor(2.5))"
    int leaf1 = 1;                        // Leaf value Int 1
    float leaf25 = 2.5;                   // Leaf value Float 2.5
    int expected = leaf1 + int(leaf25);   // Expected return value Int 3
    const FunctionSet& fs = TestFS::treeEval();
    const GpType& type_int = *(fs.lookupGpTypeByName("Int"));
    const GpType& type_float = *fs.lookupGpTypeByName("Float");
    const GpFunction& gp_func_addint = *fs.lookupGpFunctionByName("AddInt");
    const GpFunction& gp_func_floor = *fs.lookupGpFunctionByName("Floor");
    GpTree gp_tree;                       // Make empty tree.
    gp_tree.setFunction(gp_func_addint);  // Set root function to AddInt.
    gp_tree.addSubtrees(2);               // AddInt has two parameters.
    
    GpTree& st0 = gp_tree.getSubtree(0);  // Name for substree 0.
    GpTree& st1 = gp_tree.getSubtree(1);  // Name for substree 1.
    st1.addSubtrees(1);                   // "Floor" has one parameter.
    GpTree& st10 = st1.getSubtree(0);     // Name for substree 0 of st1.
    st0.setLeafValue(leaf1, type_int);    // Subtree 0 is leaf const Int 1.
    st10.setLeafValue(leaf25, type_float);// Subtree 1,0 is leaf Float 2.5.
    st1.setFunction(gp_func_floor);       // Subtree 1 has function Floor.

    return (st(&gp_tree.getType() == &type_int) &&
            st(&gp_tree.getFunction() == &gp_func_addint) &&
            st(&st1.getType() == &type_int) &&
            st(&st1.getFunction() == &gp_func_floor) &&
            st(std::any_cast<int>(gp_tree.eval()) == expected));
}

bool gp_tree_eval_objects()
{
    // Construct a tree for "ClassA(ClassB(0.5), ClassC(1, 2)"
    float leaf0_5 = 0.5;                  // Leaf value Float 0.5
    int leaf1 = 1;                        // Leaf value Int 1
    int leaf2 = 2;                        // Leaf value Int 2
    const FunctionSet& fs = TestFS::treeEvalObjects();
    const GpType& type_int = *fs.lookupGpTypeByName("Int");
    const GpType& type_float = *fs.lookupGpTypeByName("Float");
    const GpFunction& gp_func_class_a = *fs.lookupGpFunctionByName("ClassA");
    const GpFunction& gp_func_class_b = *fs.lookupGpFunctionByName("ClassB");
    const GpFunction& gp_func_class_c = *fs.lookupGpFunctionByName("ClassC");
    GpTree gp_tree;                       // Make empty tree.
    gp_tree.setFunction(gp_func_class_a); // Set root function to ClassA.
    gp_tree.addSubtrees(2);               // ClassA has two parameters.
    GpTree& st0 = gp_tree.getSubtree(0);  // Name for substree 0.
    GpTree& st1 = gp_tree.getSubtree(1);  // Name for substree 1.
    st0.addSubtrees(1);                   // "ClassB" has one parameter.
    st1.addSubtrees(2);                   // "ClassC" has two parameters.
    st0.setFunction(gp_func_class_b);     // Subtree 0 has function "ClassB".
    st1.setFunction(gp_func_class_c);     // Subtree 1 has function "ClassC".
    GpTree& st00 = st0.getSubtree(0);     // Name for substree 0 of st0.
    GpTree& st10 = st1.getSubtree(0);     // Name for substree 0 of st1.
    GpTree& st11 = st1.getSubtree(1);     // Name for substree 1 of st1.
    st00.setLeafValue(leaf0_5, type_float); // Subtree 0,0 is leaf Float 0.5.
    st10.setLeafValue(leaf1, type_int);     // Subtree 1,0 is leaf Int 1.
    st11.setLeafValue(leaf2, type_int);     // Subtree 1,1 is leaf Int 2.

    std::any result_as_any = gp_tree.eval();
    TestFS::ClassA* result = std::any_cast<TestFS::ClassA*>(result_as_any);
    std::string expected = "ClassA(ClassB(0.5), ClassC(1, 2))";
    return st(expected == result->to_string());
}

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    
    logAndTally(population_allocation_of_individuals);
    logAndTally(random_program_size_limit);
    logAndTally(gp_tree_construction);
    logAndTally(gp_tree_eval_simple);
    logAndTally(gp_tree_eval_objects);

    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}
