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
        constructor_count_++;
    }
    Individual(int max_tree_size, const FunctionSet& fs) : Individual()
    {
        fs.makeRandomTree(max_tree_size, tree_);
    }
    Individual(const GpTree& gp_tree) : Individual() { tree_ = gp_tree; }

    virtual ~Individual()
    {
        // Tell tree to delete any heap-allocated cached values it created when
        // it last did eval(). This happens via the optional "deleter" function
        // on a GpType.
        tree_.deleteCachedValues();
        getSetInstanceCount()--;
        destructor_count_++;
    }
    
    // Read-only (const) access to this Individual's GpTree.
//    const GpTree& tree() { return tree_; }
    const GpTree& tree() const { return tree_; }
    // Overwrite this Individual's GpTree with "new_tree". Flush eval() cache.
    // TODO Is this ever needed?
    void setTree(const GpTree& new_tree)
    {
        tree_ = new_tree;
        flushTreeValueCache();
    }
    // TODO Is this ever needed?
    void flushTreeValueCache()
    {
        // TODO need to delete value?
        std::cout << "WARNING: flushTreeValueCache() called" << std::endl;
        tree_evaluated_ = false;
    }
    // Return/cache the result of running/evaluating this Individual's GpTree.
    std::any treeValue()
    {
        if (!tree_evaluated_)
        {
            assert(tree_eval_counter_ == 0);
            tree_.eval();
            tree_evaluated_ = true;
            tree_eval_counter_++;
        }
        return tree_.getLeafValue();
    }
    // Get/inc count of tournament Individual has survived (did not "lose").
    int getTournamentsSurvived() const { return tournaments_survived_; }
    void incrementTournamentsSurvived() { tournaments_survived_++; }
    // Added to support "absolute fitness" in addition to "tournament fitness".
    bool hasFitness() const { return has_fitness_; }
    void setFitness(float f) { fitness_ = f; has_fitness_ = true; }
    float getFitness() const { return (hasFitness() ?
                                       fitness_ :
                                       getTournamentsSurvived()); }
    // Compare counts of constructor/destructor calls. Must match at end of run.
    static void leakCheck()
    {
        std::cout << "Individual";
        std::cout << ": constructions=" << constructor_count_;
        std::cout << ", destructions=" << destructor_count_;
        std::cout << ", leaked=" << constructor_count_ - destructor_count_;
        std::cout << std::endl;
    }

    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    static const int& getInstanceCount() { return getSetInstanceCount(); }
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
    GpTree tree_;
    // Resettable cache for result of tree evaluation.
    bool tree_evaluated_ = false;
    // Make sure we don't eval() the tree more than once. (TODO Still needed?)
    int tree_eval_counter_ = 0;
    // Number of tournament this Individual has survived (did not "lose").
    int tournaments_survived_ = 0;
    // Added to support "absolute fitness" in addition to "tournament fitness".
    float fitness_ = 0;
    bool has_fitness_ = false;
    // Leak check. Count constructor/destructor calls. Must match at end of run.
    static inline int constructor_count_ = 0;
    static inline int destructor_count_ = 0;
};
