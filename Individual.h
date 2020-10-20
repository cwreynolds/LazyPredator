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
//    Individual(int max_tree_size, const FunctionSet& fs)
//    {
//        getSetInstanceCount()++;
//        fs.makeRandomTree(max_tree_size, tree());
//    }
    Individual(int max_tree_size, const FunctionSet& fs)
    {
        getSetInstanceCount()++;
        GpTree gp_tree;
        fs.makeRandomTree(max_tree_size, gp_tree);
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO very temporary experiment to make sure generated tree is bigger
        //      than crossover_min_size
        //      ought to cycle until OK, exit after too many retries
        if (gp_tree.size() < fs.getCrossoverMinSize())
        {
            std::cout << "OOPS! generated tree size (" << gp_tree.size();
            std::cout << ") is less than crossover_min_size (";
            std::cout << fs.getCrossoverMinSize() << "). Retrying...";
            std::cout << std::endl;
            
            debugPrint(gp_tree.to_string());
            
            gp_tree = GpTree();
            fs.makeRandomTree(max_tree_size, gp_tree);
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        tree() = gp_tree;
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
    
    
//    std::any result_as_any = i->tree().eval();
    
    
    // TODO EXPERIMENTAL
    //      need to flush this cache if tree is modified
    
    
    std::any tree_value_cache_ = nullptr;
    bool tree_evaluated_ = false;
    
    // TODO name: treeValue(), treeEval(), cachedTreeEval(), ...
    std::any treeValue()
    {
        if (tree_evaluated_)
        {
//            std::cout << "using cached eval() for Individual " << this << std::endl;
        }
        else
        {
//            std::cout << "eval() tree for Individual " << this << std::endl;
            tree_value_cache_ = tree_.eval();
            tree_evaluated_ = true;
        }
        return tree_value_cache_;
    }
    
    int getTournamentsSurvived() const { return tournaments_survived_; }
    void incrementTournamentsSurvived() { tournaments_survived_++; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
    GpTree tree_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Number of tournament this Individual has survived (did not "lose").
    int tournaments_survived_ = 0;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
