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
        
    // Perform random GP crossover between the two given parents to produce a
    // new offspring, which is written into the third parameter.
    static void crossover(const GpTree& parent0,
                          const GpTree& parent1,
                          GpTree& offspring,
                          int min_size,
                          int max_size,
                          int fs_min_size)
    {
        // Randomly assign parent0/parent1 to donor/recipient roles.
        bool exchange = LPRS().randomBool();
        // The "donor" is a copy of one parent, selected randomly.
        GpTree donor = exchange ? parent0 : parent1;
        // The offspring is initialized to a copy of the other parent.
        offspring = exchange ? parent1 : parent0;
        // Perform actual crossover.
        crossoverDonorRecipient(donor, offspring, min_size, max_size, fs_min_size);
    }

    static void crossoverDonorRecipient(GpTree& donor,
                                        GpTree& recipient,
                                        int min_size,
                                        int max_size,
                                        int fs_min_size)
    {
        // Find set of GpTypes which is common to both parents.
        std::set<const GpType*> types;
        GpTree::sharedSetOfTypes(donor, recipient, types);
        assert(!types.empty());
        // Basic crossover invoked for each of the three size cases below.
        auto crosser = [&](int d_size_bias, int r_size_bias)// -1, 0, +1 (enum?)
        {
            // Pick a crossover subtree in the donor tree. Must return one of
            // the common types, must be larger than the FunctionSet's min_size,
            // and respect the given donor size bias.
            GpTree& d_subtree = donor.selectCrossoverSubtree(fs_min_size,
                                                             d_size_bias,
                                                             types);
            // Pick a crossover subtree in the donor tree. Must return the same
            // type as d_subtree, must be larger than the FunctionSet's
            // min_size, and respect the given recipient size bias.
            std::set<const GpType*> donor_type = { d_subtree.getRootType() };
            GpTree& r_subtree = recipient.selectCrossoverSubtree(fs_min_size,
                                                                 r_size_bias,
                                                                 donor_type);
            assert(d_subtree.getRootType() == r_subtree.getRootType());
            // Overwrite the recipient subtree with copy of donor subtree.
            r_subtree = d_subtree;
        };
        // If "recipient" too big/small, try to fix via relative subtree size.
        // In each case, select random subtree from "donor" and "recipient"
        // (with size bias when needed).
        int r_size = recipient.size();
        assert(min_size <= max_size);
        if (r_size > max_size)
        {                        // If recipient too big, small donor subtree
            crosser(-1, +1);     // replaces a big recipient subtree.
        }
        else if (r_size < min_size)
        {                        // If recipient too small, big donor subtree
            crosser(+1, -1);     // replaces a small recipient subtree.
        }
        else
        {
            crosser(0, 0);       // Otherwise uniform random subtree selection.
        }
    }
    
    // Randomly select a subtree of this GpTree to be used for crossover. Its
    // size must be at least "min_size". Its root type must be a member of the
    // set "types". If "size_bias" is not zero, attempts to find a big (+1) or
    // small (-1) subtree.
    GpTree& selectCrossoverSubtree(int min_size,
                                   int size_bias,  // -1, 0, +1 (enum?)
                                   const std::set<const GpType*>& types)
    {
        GpTree* result = this;
        auto gp_type_ok = [&](GpTree* tree)
            { return set_contains(types, tree->getRootType()); };
        if (size() > min_size)
        {
            // Find all subtrees.
            std::vector<GpTree*> all_subtrees;
            collectVectorOfSubtrees(all_subtrees);
            // Filter collection of subtrees by type and size.
            std::vector<GpTree*> filtered_subtrees;
            for (auto& gp_tree : all_subtrees)
            {
                if (gp_type_ok(gp_tree) && (gp_tree->size() >= min_size))
                {
                    filtered_subtrees.push_back(gp_tree);
                }
            }
            assert(!filtered_subtrees.empty());
            if (size_bias == 0)
            {
                // For the no bias case, simply pick one of these at random.
                result = LPRS().randomSelectElement(filtered_subtrees);
            }
            else
            {
                // To bias for size, sort filtered subtree collection by size,
                // select from upper or lower 1/3 of the sorted list.
                std::sort(filtered_subtrees.begin(),
                          filtered_subtrees.end(),
                          [](GpTree* a,GpTree* b){return a->size()<b->size();});
                int end = int(filtered_subtrees.size()) - 1;
                int from = (size_bias < 0) ? 0 : ((2 * end) / 3);  // 0 or 2/3
                int to   = (size_bias < 0) ? (end / 3) : end;      // 1/3 or 1
                result = filtered_subtrees.at(LPRS().random2(from, to));
            }
        }
        assert(gp_type_ok(result));
        return *result;
    }

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
