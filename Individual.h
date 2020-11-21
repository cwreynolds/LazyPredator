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
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
        // TODO very temp, trying to debug delete of instances in tree
        constructed_set_.insert(this);
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    }
    Individual(int max_tree_size, const FunctionSet& fs) : Individual()
    {
        fs.makeRandomTree(max_tree_size, tree_);
    }
    Individual(const GpTree& gp_tree) : Individual() { tree_ = gp_tree; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    ~Individual() { getSetInstanceCount()--; }
    ~Individual()
    {
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
        // TODO very temp, trying to debug delete of instances in tree
        assert(set_contains(constructed_set_, this));
        assert(!set_contains(destructed_set_, this));
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO use experimental "deleter" function. EG for Texture in TexSyn.
        tree_.deleteCachedValues();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        getSetInstanceCount()--;
        destructor_count_++;
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
        // TODO very temp, trying to debug delete of instances in tree
        destructed_set_.insert(this);
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Read-only (const) access to this Individual's GpTree.
    const GpTree& tree() { return tree_; }
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
        tree_value_cache_ = nullptr;
        tree_evaluated_ = false;
    }
    // Return/cache the result of running/evaluating this Individual's GpTree.
    std::any treeValue()
    {
        if (!tree_evaluated_)
        {
            tree_value_cache_ = tree_.eval();
            tree_evaluated_ = true;
        }
        return tree_value_cache_;
    }
    // Get/inc count of tournament Individual has survived (did not "lose").
    int getTournamentsSurvived() const { return tournaments_survived_; }
    void incrementTournamentsSurvived() { tournaments_survived_++; }
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    bool hasFitness() const { return has_fitness_; }
    void setFitness(float f) { fitness_ = f; has_fitness_ = true; }
//    float getFitness() const { assert(hasFitness()); return fitness_; }
    float getFitness() const { return (hasFitness() ?
                                       fitness_ :
                                       getTournamentsSurvived()); }

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
    std::any tree_value_cache_ = nullptr;
    bool tree_evaluated_ = false;
    // Number of tournament this Individual has survived (did not "lose").
    int tournaments_survived_ = 0;
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    float fitness_ = 0;
    bool has_fitness_ = false;
    
    static inline int constructor_count_ = 0;
    static inline int destructor_count_ = 0;
    
    // TODO very temp, trying to debug delete of instances in tree
    static inline std::set<Individual*> constructed_set_;
    static inline std::set<Individual*> destructed_set_;
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
};
