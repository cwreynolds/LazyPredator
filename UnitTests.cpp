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

bool random_program_size_limit()
{
    // TODO I tried this on August 21 with make_full_texsyn_fs() and it failed.
    // But I am switching over from old-style to GpType and GpFunction so may be
    // related to that. Eventually want to use TexSyn API here (plus others?)
    FunctionSet fs(
    {
        {"Float_01", [](){ return frandom01() < 0.5 ? "0" : "1"; }},
        {"Texture"},
        {"Vec2"}
    },
    {
        {"Vec2", "Vec2",
            {"Float_01", "Float_01"}},
        {"Uniform", "Texture",
            {"Float_01", "Float_01", "Float_01"}},
        {"Spot", "Texture",
            {"Vec2", "Float_01", "Texture", "Float_01", "Texture"}},
        {"Add", "Texture", {"Texture", "Texture"}},
        {"Blur", "Texture", {"Float_01", "Texture"}},
        {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}}
    });
    
    bool all_ok = true;
    int total_subtests = 1000;
    RandomSequence rs(77365918);
    for (int i = 0; i < total_subtests; i++)
    {
        int max_size = int(rs.frandom2(4, 100));
        int actual_size = 0;
        std::string source_code;
        fs.dp_depth = 0;
        fs.makeRandomProgram(max_size, "Texture", actual_size, source_code);
        if (!st(actual_size <= max_size)) all_ok = false;
    }
    return all_ok;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool test_gp_tree_construction()
{
    GpTree root;
    root.setId("r");
    bool created_empty = st(root.subtrees().empty());
    
    GpTree& subtree_1 = root.addSubtree();
    bool count_1 = st(root.subtrees().size() == 1);
    subtree_1.setId("1");
    
    GpTree& subtree_2 = root.addSubtree();
    bool count_2 = st(root.subtrees().size() == 2);
    subtree_2.setId("2");
    
    GpTree& subtree_2a = subtree_2.addSubtree();
    subtree_2a.setId("2a");

    debugPrint(root.id());
    debugPrint(subtree_1.id());
    debugPrint(subtree_2.id());
    debugPrint(subtree_2a.id());
    
    debugPrint(&root);
    debugPrint(&subtree_1);
    debugPrint(&subtree_2);
    debugPrint(&subtree_2a);

    return (created_empty &&
            count_1 &&
            count_2 &&
            
            st(root.id() == "r") &&
            st(subtree_1.id() == "1") &&
            st(subtree_2.id() == "2") &&
            st(subtree_2a.id() == "2a") &&

            true);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    
    logAndTally(mock);
    logAndTally(population_allocation_of_individuals);
    logAndTally(random_program_size_limit);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    logAndTally(test_gp_tree_construction);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}
