//
//  GpTree.h
//  LazyPredator
//
//  Created by Craig Reynolds on 12/18/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"
#include "GpType.h"
#include "GpFunction.h"

// GpTree: a "program tree", an "abstract syntax tree" ("AST"), to represent a
// composition of GpFunction(s) and GpType(s). Each GpTree instance contains a
// vector of subtrees, for each parameter of the function at the root, and so
// on recursively, so as to contain the whole tree.
class GpTree
{
public:
    // Default costructor.
    GpTree(){}
    // Reference to vector of subtrees, const or not.
    std::vector<GpTree>& subtrees() { return subtrees_; }
    const std::vector<GpTree>& subtrees() const { return subtrees_; }
    // Get reference to i-th subtree. Like: subtrees().at(i)
    GpTree& getSubtree(int i) { return subtrees().at(i); }
    const GpTree& getSubtree(int i) const { return subtrees().at(i); }
    // Get/set reference to GpFunction object at root of this tree.
    const GpFunction& getRootFunction() const { return *root_function_; }
    void setRootFunction(const GpFunction& function)
    {
        root_function_ = &function;
        root_type_ = function.returnType();
    }
    // Get const pointer to GpType returned by this GpTree node/subtree.
    const GpType* getRootType() const { return root_type_; }
    // Add (allocate) "count" new subtrees. Should only be called once.
    // TODO maybe instead of "add" call it "declare" or "init"?
    void addSubtrees(size_t count)
    {
        assert("call addSubtrees() only once" && subtrees().size() == 0);
        for (int i = 0; i < count; i++) addSubtree();
    }
    // Count tokens in tree (functions or leaves/constants).
    int size() const
    {
        int count = 1;
        for (auto& subtree : subtrees()) count += subtree.size();
        return count;
    }
    // Get/set the value at the root of this GpTree. This can be either:
    // (a) a constant "leaf value" of a GpTree with no subtrees and no root
    //     function, as set during makeRandomTree().
    // (b) a cached value at the root of a GpTree with a "root function" that
    //     is calculated during eval().
    // Note that setting the value requires a GpType be specified.
    std::any getRootValue() const { return leaf_value_; }
    void setRootValue(std::any value, const GpType& gp_type)
    {
        // Verify new type matches old type, if any.
        if (getRootType()) assert(getRootType() == &gp_type);
        leaf_value_ = value;
        root_type_ = &gp_type;
    }
    // A GpTree is a "leaf node" if it has no GpFunction at its root.
    bool isLeaf() const { return !root_function_; }
    // Evaluate this tree. Run/evaluate the GpFunction at the root, recursively
    // running (evaluating) each parameter subtree. Stores value at each node.
    std::any eval()
    {
        if (!isLeaf())
        {
            setRootValue(getRootFunction().eval(*this),
                         *getRootFunction().returnType());
        }
        return getRootValue();
    }
    // Evaluate i-th subtree, corresponds to i-th parameter of root function,
    // then cast the resulting std::any to the given concrete type T.
    template <typename T> T evalSubtree(int i)
    {
        return std::any_cast<T>(getSubtree(i).eval());
    }    
    // Convert this GpTree to "source code" format as a string. It is either a
    // single constant "leaf" value, or the root function's name followed by a
    // parenthesized, comma separated, list of parameter trees.
    //
    // Optional "indent" flag to include line breaks and indentation.
    // Optional "prefix" string to include at beginning of each line.
    //
    std::string to_string() const { return to_string_helper(false, "", 0); }
    std::string to_string(bool indent) const
        { return to_string_helper(indent, "", 0); }
    std::string to_string(bool indent, const std::string& prefix) const
        { return to_string_helper(indent, prefix, 0); }
    std::string to_string_helper(bool indent,
                                 const std::string& prefix,
                                 int indentation) const
    {
        std::string s;
        if (indentation == 0) s = prefix;
        auto new_line = [&]()
        {
            if (indent)
            {
                s += "\n";
                s += prefix;
                for (int i = 0; i < indentation; i++) s += " ";
            }
            else
            {
                s += " ";
            }
        };
        auto all_leaves = [&]()
        {
            bool all = true;
            for (auto& subtree : subtrees()) if (!subtree.isLeaf()) all = false;
            return all;
        };
        if (isLeaf())
        {
            s += getRootType()->to_string(getRootValue());
        }
        else
        {
            bool indent_before = indent;
            if (all_leaves()) { indent = false; }
            size_t additional_indent = getRootFunction().name().size() + 1;
            indentation += additional_indent;
            s += getRootFunction().name() + "(";
            bool comma = false;
            for (auto& subtree : subtrees())
            {
                if (comma) { s += ","; new_line(); } else { comma = true; }
                s += subtree.to_string_helper(indent, prefix, indentation);
            }
            s += ")";
            indentation -= additional_indent;
            indent = indent_before;
        }
        return s;
    }

    // Collect all subtrees into an std::vector. Recursively traverses tree from
    // this root down, storing pointers to each subtree into vector.
    // NOTE: assumes vector is empty before initial call.
    void collectVectorOfSubtrees(std::vector<GpTree*>& all_subtrees_output)
    {
        all_subtrees_output.push_back(this);
        for (auto& subtree : subtrees())
            subtree.collectVectorOfSubtrees(all_subtrees_output);
    }

    // Collect all subtree typess into an std::set. Recursively traverses tree
    // from this root down, inserting pointers to each GpType into set.
    // NOTE: assumes set is empty before initial call.
    void collectSetOfTypes(std::set<const GpType*>& set_of_types_output) const
    {
        set_of_types_output.insert(getRootType());
        for (auto& subtree : subtrees())
            subtree.collectSetOfTypes(set_of_types_output);
    }
    
    // Given two trees find the set of types common to both. That is: collect
    // the set of types for each then take the intersection of those types.
    static void sharedSetOfTypes(const GpTree& a,
                                 const GpTree& b,
                                 std::set<const GpType*>& set_of_types_output)
    {
        std::set<const GpType*> a_types;
        std::set<const GpType*> b_types;
        a.collectSetOfTypes(a_types);
        b.collectSetOfTypes(b_types);
        set_of_types_output.clear();
        std::set_intersection(a_types.begin(), a_types.end(),
                              b_types.begin(), b_types.end(),
                              std::inserter(set_of_types_output,
                                            set_of_types_output.begin()));
    }
    
    // Utility for crossover(). Select a random subtree, whose size is greater
    // then or equal to "min_size" and whose root type is in set of "types".
    // (Default to root if entire tree is not larger than "min_size". See final
    // paragraph on https://cwreynolds.github.io/LazyPredator/#20201019)
    GpTree& selectCrossoverSubtree(int min_size,
                                   const std::set<const GpType*>& types)
    {
        GpTree* result = this;
        auto gp_type_ok = [&](GpTree* tree)
            { return set_contains(types, tree->getRootType()); };
        if (size() > min_size)
        {
            std::vector<GpTree*> all_subtrees;
            collectVectorOfSubtrees(all_subtrees);
            std::vector<GpTree*> filtered_subtrees;
            for (auto& gp_tree : all_subtrees)
            {
                if (gp_type_ok(gp_tree) && (gp_tree->size() >= min_size))
                {
                    filtered_subtrees.push_back(gp_tree);
                }
            }
            assert(!filtered_subtrees.empty());
            result = LPRS().randomSelectElement(filtered_subtrees);
        }
        assert(gp_type_ok(result));
        return *result;
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210114 version of GpTree::selectCrossoverSubtree() with max_size
    
    // Utility for crossover(). Select a random subtree, whose size is greater
    // then or equal to "min_size" and whose root type is in set of "types".
    // (Default to root if entire tree is not larger than "min_size". See final
    // paragraph on https://cwreynolds.github.io/LazyPredator/#20201019)
    GpTree& selectCrossoverSubtree(int min_size,
                                   int max_size,
                                   const std::set<const GpType*>& types)
    {
        GpTree* result = this;
        auto gp_type_ok = [&](GpTree* tree)
            { return set_contains(types, tree->getRootType()); };
        if (size() > min_size)
        {
            std::vector<GpTree*> all_subtrees;
            collectVectorOfSubtrees(all_subtrees);
            std::vector<GpTree*> filtered_subtrees;
            for (auto& gp_tree : all_subtrees)
            {
//                if (gp_type_ok(gp_tree) && (gp_tree->size() >= min_size))
                if (gp_type_ok(gp_tree) &&
                    (gp_tree->size() >= min_size) &&
                    (gp_tree->size() <= max_size))
                {
                    filtered_subtrees.push_back(gp_tree);
                }
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20210114 working on min/max crossover limits VERY TEMP
            if (filtered_subtrees.empty())
            {
                debugPrint(min_size);
                debugPrint(max_size);
                std::cout << "types: ";
                for (auto& type : types) { std::cout << type->name() << " "; }
                std::cout << std::endl;
                std::cout << to_string(true) << std::endl;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            assert(!filtered_subtrees.empty());
            result = LPRS().randomSelectElement(filtered_subtrees);
        }
        assert(gp_type_ok(result));
        return *result;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Traverse tree, applying "jiggle" point mutation on each constant leaf
    // value, according to it GpType.
    // TODO note makeRandomTree() and crossover() are methods of FunctionSet
    //      Is this misplaced, or are they?
    void mutate()
    {
        if (isLeaf())
        {
            setRootValue(getRootType()->jiggleConstant(getRootValue()),
                         *getRootType());
        }
        else
        {
            for (auto& subtree : subtrees()) subtree.mutate();
        }
    }
    // Essentially like operator==() but needs to be a template for the sake of
    // the std::any leaf nodes. Used only in the unit tests.
    template <typename T> static bool match(const GpTree& a, const GpTree& b)
    {
        auto equal_subtrees = [](const GpTree& a, const GpTree& b)
        {
            bool ok = a.subtrees_.size() == b.subtrees_.size();
            for (int i = 0; i < a.subtrees_.size(); i++)
                if (!match<T>(a.getSubtree(i), b.getSubtree(i))) ok = false;
            return ok;
        };
        return (a.root_function_ == b.root_function_ && // Root functions match.
                a.root_type_ == b.root_type_ &&         // Root types match.
                a.id_ == b.id_ &&                       // IDs match.
                equal_subtrees(a, b) &&                 // All subtrees match.
                (!(a.isLeaf() && b.isLeaf()) ||         // Both not leaves, or
                 (std::any_cast<T>(a.leaf_value_) ==    //   both leaves match.
                  std::any_cast<T>(b.leaf_value_))));
    }

    void deleteCachedValues()
    {
        auto rt = getRootType();
        if (rt && rt->hasDeleter()) { rt->deleteValue(getRootValue()); }
        for (auto& subtree : subtrees()) subtree.deleteCachedValues();
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210111 new crossover() with min/max tree size
    // TODO should crossover() be on GpTree?
    
//        static void newCrossover(const GpTree& parent0,
//                                 const GpTree& parent1,
//                                 GpTree& offspring,
//                                 int min_size,
//                                 int max_size)
//        {
//            bool exchange = LPRS().frandom01() > 0.5;
//            // The "donor" is a copy of one parent, selected randomly.
//            GpTree donor = exchange ? parent0 : parent1;
//            // The offspring is initialized to a copy of the other parent.
//            offspring = exchange ? parent1 : parent0;
//
//            newCrossoverOrdered(donor, offspring, min_size, max_size);
//        }
//
//        static void newCrossoverOrdered(GpTree& donor,
//                                        GpTree& recipient,
//                                        int min_size,
//                                        int max_size)
//        {
//    //        bool exchange = LPRS().frandom01() > 0.5;
//    //        // The "donor" is a copy of one parent, selected randomly.
//    //        GpTree donor = exchange ? parent0 : parent1;
//    //        // The offspring is initialized to a copy of the other parent.
//    //        offspring = exchange ? parent1 : parent0;
//            // Find set of GpTypes which is common to both parents.
//            std::set<const GpType*> types;
//    //        GpTree::sharedSetOfTypes(parent0, parent1, types);
//            GpTree::sharedSetOfTypes(donor, recipient, types);
//            assert(!types.empty());
//            // Select random subtree from donor (=> min-size, any shared_type).
//    //        int min_size = getCrossoverMinSize();
//    //        min_size = std::max(min_size, getCrossoverMinSize());
//            GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
//            // Select random subtree from offspring (any size, same type as dst).
//            std::set<const GpType*> donor_type = { d_subtree.getRootType() };
//    //        GpTree& o_subtree = offspring.selectCrossoverSubtree(1, donor_type);
//            GpTree& o_subtree = recipient.selectCrossoverSubtree(1, donor_type);
//            // Set offspring subtree to copy of donor subtree.
//            o_subtree = d_subtree;
//        }
    
    // Perform random GP crossover between the two given parents to produce a
    // new offspring, which is written into the third parameter.
    static void crossover(const GpTree& parent0,
                          const GpTree& parent1,
                          GpTree& offspring,
                          int min_size,
                          int max_size)
    {
        // Randomly assign parent0/parent1 to donor/recipient roles.
        // TODO this should use randomBool() but lets wait to preserve
        //      "identical result" regression test for a while
        bool exchange = LPRS().frandom01() > 0.5;
        // The "donor" is a copy of one parent, selected randomly.
        GpTree donor = exchange ? parent0 : parent1;
        // The offspring is initialized to a copy of the other parent.
        offspring = exchange ? parent1 : parent0;
        
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20210115 rethink max/min crossover

        // Perform actual crossover.
//        crossover(donor, offspring, min_size, max_size);
        
        // TODO NOTE TEMP PROTPYPE OF fs_min_size
        crossoverDonorRecipient(donor, offspring, min_size, max_size, 4);

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    
//    static void crossover(GpTree& donor,
//                          GpTree& recipient,
//                          int min_size,
//                          int max_size)
//    {
//        // Find set of GpTypes which is common to both parents.
//        std::set<const GpType*> types;
//        GpTree::sharedSetOfTypes(donor, recipient, types);
//        assert(!types.empty());
//        // Select random subtree from donor (=> min-size, any shared_type).
//        GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
//        // Select random subtree from offspring (any size, same type as dst).
//        std::set<const GpType*> donor_type = { d_subtree.getRootType() };
//        GpTree& o_subtree = recipient.selectCrossoverSubtree(1, donor_type);
//        // Set offspring subtree to copy of donor subtree.
//        o_subtree = d_subtree;
//    }

//        static void crossover(GpTree& donor,
//                              GpTree& recipient,
//                              int min_size,
//                              int max_size)
//        {
//            // Find set of GpTypes which is common to both parents.
//            std::set<const GpType*> types;
//            GpTree::sharedSetOfTypes(donor, recipient, types);
//            assert(!types.empty());
//            // Select random subtree from donor (=> min-size, any shared_type).
//            GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
//            // Select random subtree from offspring (any size, same type as dst).
//            std::set<const GpType*> donor_type = { d_subtree.getRootType() };
//    //        GpTree& o_subtree = recipient.selectCrossoverSubtree(1, donor_type);
//            GpTree& r_subtree = recipient.selectCrossoverSubtree(1, donor_type);
//            // Set offspring subtree to copy of donor subtree.
//    //        o_subtree = d_subtree;
//            r_subtree = d_subtree;
//        }

//        static void crossover(GpTree& donor,
//                              GpTree& recipient,
//                              int min_size,
//                              int max_size)
//        {
//            // Find set of GpTypes which is common to both parents.
//            std::set<const GpType*> types;
//            GpTree::sharedSetOfTypes(donor, recipient, types);
//            assert(!types.empty());
//
//            // Select random subtree from donor (=> min-size, any shared_type).
//            GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
//
//            // Select random subtree from offspring (any size, same type as dst).
//            std::set<const GpType*> donor_type = { d_subtree.getRootType() };
//            int r_size = 1;
//
//            // If recipient is overweight, require that the recipient subtree be
//            // larger that the donor subtree, hence reducing size of the offspring.
//            if (recipient.size() > max_size) { r_size = d_subtree.size(); }
//
//    //        GpTree& r_subtree = recipient.selectCrossoverSubtree(1, donor_type);
//            GpTree& r_subtree = recipient.selectCrossoverSubtree(r_size, donor_type);
//            // Set offspring subtree to copy of donor subtree.
//
//    //        // TODO just for debugging:
//    //        if (recipient.size() > max_size)
//    //        {
//    //            std::cout << "-----------------------------" << std::endl;
//    //            debugPrint(recipient.size());
//    //            debugPrint(d_subtree.size());
//    //            debugPrint(r_subtree.size());
//    //            std::cout << "-----------------------------" << std::endl;
//    //        }
//
//
//            r_subtree = d_subtree;
//        }

    
    static void crossover(GpTree& donor,
                          GpTree& recipient,
                          int min_size,
                          int max_size)
    {
        // Find set of GpTypes which is common to both parents.
        std::set<const GpType*> types;
        GpTree::sharedSetOfTypes(donor, recipient, types);
        assert(!types.empty());
        
        // Select random subtree from donor (=> min-size, any shared_type).
//        GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
        // TODO -- VERY VERY TEMP ***********************************************
        GpTree& d_subtree = donor.selectCrossoverSubtree(4, types);

        // Select random subtree from offspring (any size, same type as dst).
        std::set<const GpType*> donor_type = { d_subtree.getRootType() };
//        int r_size = 1;
        int r_min_size = 1;
        int r_max_size = std::numeric_limits<int>::max();

        // If recipient is overweight, require that the recipient subtree be
        // larger that the donor subtree, hence decreasing offspring size.
//        if (recipient.size() > max_size) { r_size = d_subtree.size(); }
        if (recipient.size() > max_size) { r_min_size = d_subtree.size(); }

        // If recipient is underweight, require that the recipient subtree be
        // smaller that the donor subtree, hence increasing offspring size.
        if (recipient.size() < min_size) { r_max_size = d_subtree.size(); }
        
//        GpTree& r_subtree = recipient.selectCrossoverSubtree(r_size, donor_type);
        GpTree& r_subtree = recipient.selectCrossoverSubtree(r_min_size,
                                                             r_max_size,
                                                             donor_type);
        
        
        // TODO just for debugging:
        if ((recipient.size() > max_size) || (recipient.size() < min_size))
        {
            std::cout << "-----------------------------" << std::endl;
            debugPrint(recipient.size());
            debugPrint(min_size);
            debugPrint(max_size);
            debugPrint(r_min_size);
            debugPrint(r_max_size);
            debugPrint(d_subtree.size());
            debugPrint(r_subtree.size());
            
            std::cout << ((d_subtree.size() > r_subtree.size()) ?
                          "Increase" : "Decrease")
                      << " size." << std::endl;

            std::cout << "-----------------------------" << std::endl;
        }

        
        // Set offspring subtree to copy of donor subtree.
        r_subtree = d_subtree;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210115 rethink max/min crossover
    static void crossoverDonorRecipient(GpTree& donor,
                                        GpTree& recipient,
                                        int min_size,
                                        int max_size,
                                        int fs_min_size)
    {
        assert(min_size <= max_size);

        // Find set of GpTypes which is common to both parents.
        std::set<const GpType*> types;
        GpTree::sharedSetOfTypes(donor, recipient, types);
        assert(!types.empty());
        
//        // Select random subtree from donor (=> min-size, any shared_type).
//        GpTree& d_subtree = donor.selectCrossoverSubtree(fs_min_size, types);
        
        
        // TODO TEMP just for debugging:
        auto log = [&](const GpTree& d_subtree, const GpTree& r_subtree)
        {
            std::cout << "-----------------------------" << std::endl;
            debugPrint(donor.size());
            debugPrint(recipient.size());
            debugPrint(min_size);
            debugPrint(max_size);
            debugPrint(fs_min_size);
            debugPrint(d_subtree.size());
            debugPrint(r_subtree.size());
//            std::cout << ((d_subtree.size() > r_subtree.size()) ?
//                          "Increase" : "Decrease") << " size." << std::endl;
            std::cout << ((d_subtree.size() > r_subtree.size()) ?
                          "Increase" : "Decrease") << " size from " <<
                          recipient.size() << " to " << (recipient.size() -
                          r_subtree.size() + d_subtree.size()) << std::endl;
            std::cout << "-----------------------------" << std::endl;
        };

        // If "recipient" too big/small, try to fix via relative subtree size.
        // In each case, select random subtree from "donor" and "recipient"
        // (with size bias when needed) then set offspring recipient to copy of
        // donor subtree.

        int r_size = recipient.size();
        if (r_size > max_size)
        {
            GpTree& dst = donor.selectCrossoverSubtreeNEW(fs_min_size, -1, types);
            GpTree& rst = recipient.selectCrossoverSubtreeNEW(fs_min_size, +1, types);
            log(dst, rst);
            rst = dst;
        }
        else if (r_size < min_size)
        {
            GpTree& dst = donor.selectCrossoverSubtreeNEW(fs_min_size, +1, types);
            GpTree& rst = recipient.selectCrossoverSubtreeNEW(fs_min_size, -1, types);
            log(dst, rst);
            rst = dst;
        }
        else
        {
            GpTree& dst = donor.selectCrossoverSubtreeNEW(fs_min_size, 0, types);
            GpTree& rst = recipient.selectCrossoverSubtreeNEW(fs_min_size, 0, types);
            rst = dst;
        }
        

    }
    
    // TODO do we still need
    
    GpTree& selectCrossoverSubtreeNEW(int min_size,
                                      int size_bias,  // -1, 0, +1 (enum?)
                                      const std::set<const GpType*>& types)
    {
        GpTree* result = this;
        auto gp_type_ok = [&](GpTree* tree)
            { return set_contains(types, tree->getRootType()); };
        if (size() > min_size)
        {
            std::vector<GpTree*> all_subtrees;
            collectVectorOfSubtrees(all_subtrees);
            std::vector<GpTree*> filtered_subtrees;
            for (auto& gp_tree : all_subtrees)
            {
                if (gp_type_ok(gp_tree) && (gp_tree->size() >= min_size))
                {
                    filtered_subtrees.push_back(gp_tree);
                }
            }
            assert(!filtered_subtrees.empty());
            
//            result = LPRS().randomSelectElement(filtered_subtrees);
            
            
            if (size_bias == 0)
            {
                result = LPRS().randomSelectElement(filtered_subtrees);
            }
            else
            {
                std::sort(filtered_subtrees.begin(),
                          filtered_subtrees.end(),
                          [](GpTree* a,GpTree* b){return a->size()<b->size();});
                int end = int(filtered_subtrees.size()) - 1;
                
                // TODO was using [0, 1/3] and [2/3, 1] now: [0, 1/2] [1/2, 1]
//                int a = end / 3;
//                int b = 2 * (end / 3);
//                int a = end / 2;
//                int b = a;
                // TODO back
                int a = end / 3;
                int b = (2 * end) / 3;
                if (size_bias > 0)
                {
                    result = filtered_subtrees.at(LPRS().random2(b, end));
                }
                else
                {
                    result = filtered_subtrees.at(LPRS().random2(0, a));

                }
            }
        }
        assert(gp_type_ok(result));
        return *result;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
    // NOTE: if any more data members are added, compare them in equals().
    // Add (allocate) one subtree. addSubtrees() is external API.
    void addSubtree() { subtrees_.push_back({}); }
    // Each GpTree root will have a function object or a leaf value.
    const GpFunction* root_function_ = nullptr;
    const GpType* root_type_ = nullptr;
    std::any leaf_value_;
    std::vector<GpTree> subtrees_;
    std::string id_;                            // TODO for debugging only.
};
