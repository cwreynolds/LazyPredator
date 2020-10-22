//
//  Individual.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"
#include "FunctionSet.h"

class Individual
{
public:
    Individual()
    {
        getSetInstanceCount()++;
    }
    Individual(int max_tree_size, const FunctionSet& fs)
    {
        getSetInstanceCount()++;
        fs.makeRandomTree(max_tree_size, tree());
    }
    ~Individual()
    {
        getSetInstanceCount()--;
    }
    // TODO was private non const accessor, but need access to modify. Maybe
    //      make a setTree() function that copies in the given tree?
    GpTree& tree() { return tree_; }
    static const int& getInstanceCount() { return getSetInstanceCount(); }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Return the result of running/evaluating this Individual's GpTree.
    // TODO EXPERIMENTAL need to flush this cache if tree is modified
    std::any treeValue()
    {
        if (!tree_evaluated_)
        {
            tree_value_cache_ = tree_.eval();
            tree_evaluated_ = true;
        }
        return tree_value_cache_;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Get/inc count of tournament Individual has survived (did not "lose").
    int getTournamentsSurvived() const { return tournaments_survived_; }
    void incrementTournamentsSurvived() { tournaments_survived_++; }
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
    GpTree tree_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Resettable cache for result of tree evaluation.
    std::any tree_value_cache_ = nullptr;
    bool tree_evaluated_ = false;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Number of tournament this Individual has survived (did not "lose").
    int tournaments_survived_ = 0;
};
