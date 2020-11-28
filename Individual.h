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
//        // TODO very temp, trying to debug delete of instances in tree
//        std::cout << "construct Individual this=" << this << std::endl;
//        assert(!set_contains(constructed_set_, this));
//        constructed_set_.insert(this);
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    }
    Individual(int max_tree_size, const FunctionSet& fs) : Individual()
    {
        fs.makeRandomTree(max_tree_size, tree_);
        
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201121 random experiment, is it possible the GpTree of some
//        //               Individual is not evaluated/cached?
//        assert(treeValue().has_value());
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201123 temporary for testing (specific to TexSyn).
//        tree().verifyTexturePointers();
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201122 -- very temp -- validate
//        Texture* texture = std::any_cast<Texture*>(treeValue());
//        assert(texture->valid());
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201124 qqq
//        debugPrint(Individual::constructor_count_);
//        Texture* texture = std::any_cast<Texture*>(treeValue());
//        assert(texture->valid());
        
//        assert(getTexture()->valid());
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201123 temporary for testing (specific to TexSyn).
        // TODO 20201126 compare to_string() before and after.
//        std::string before = tree().to_string();//20201126 compare before/after.
        // OH! but this is only looking at the top level, the bad_any is inside
        assert(treeValue().has_value());
//        std::string after = tree().to_string();//20201126 compare before/after.
//        assert(before == after);               //20201126 compare before/after.
        
//        // TODO 20201126 see whole tree.
//        if (constructor_count_ == 10) debugPrint(after);
        
        validateInitialTreeValue();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201123 temporary for testing (specific to TexSyn).

//    Individual(const GpTree& gp_tree) : Individual() { tree_ = gp_tree; }
    Individual(const GpTree& gp_tree) : Individual()
    {
        tree_ = gp_tree;
        validateInitialTreeValue();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    ~Individual() { getSetInstanceCount()--; }
    ~Individual()
    {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO use experimental "deleter" function. EG for Texture in TexSyn.
        tree_.deleteCachedValues();
        
        // TODO 20201121 experimental clear all state
        tree_.clear();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        getSetInstanceCount()--;
        destructor_count_++;
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
//        // TODO very temp, trying to debug delete of instances in tree
//        destructed_set_.insert(this);
//        std::cout << "destruct Individual this=" << this << std::endl;
        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201123 temporary for testing (specific to TexSyn).
    //               validate value of initial GpTree
    void validateInitialTreeValue() const
    {
        tree().verifyTexturePointers();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201122 change to just get cached state from GpTree
//        tree_value_cache_ = nullptr;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        tree_evaluated_ = false;
    }
    // Return/cache the result of running/evaluating this Individual's GpTree.
    std::any treeValue()
    {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201122 change to just get cached state from GpTree
//        if (!tree_evaluated_)
//        {
//            tree_value_cache_ = tree_.eval();
//            tree_evaluated_ = true;
//        }
//        return tree_value_cache_;
                
        if (!tree_evaluated_)
        {
            assert(tree_eval_counter_ == 0);
            tree_.eval();
            tree_evaluated_ = true;
            tree_eval_counter_++;
        }
        return tree_.getLeafValue();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // TODO 20201123 temporary utility for testing (specific to TexSyn).
//    // like GP::textureFromIndividual()
//    Texture* getTexture()
//    {
//        Texture* texture = std::any_cast<Texture*>(tree_.eval());
//        assert(texture->valid());
//        return texture;
//    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
    GpTree tree_;
    // Resettable cache for result of tree evaluation.
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // TODO 20201122 change to just get cached state from GpTree
//    std::any tree_value_cache_ = nullptr;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    bool tree_evaluated_ = false;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201124 make sure we don't eval() the tree more than once.
    int tree_eval_counter_ = 0;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Number of tournament this Individual has survived (did not "lose").
    int tournaments_survived_ = 0;
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    float fitness_ = 0;
    bool has_fitness_ = false;
    
    static inline int constructor_count_ = 0;
    static inline int destructor_count_ = 0;
    
//    // TODO very temp, trying to debug delete of instances in tree
//    static inline std::set<Individual*> constructed_set_;
//    static inline std::set<Individual*> destructed_set_;
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
};
