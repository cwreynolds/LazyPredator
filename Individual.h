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
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
    GpTree tree_;
};
