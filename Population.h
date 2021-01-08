//
//  Population.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Individual.h"
#include "FunctionSet.h"
#include "TournamentGroup.h"
#include <iomanip>

class Population
{
public:
    Population() : Population(0, 1, 0, nullptr) {}
    Population(int individual_count)
      : Population(individual_count, 1, 0, nullptr) {}
    Population(int individual_count, int max_tree_size, const FunctionSet& fs)
      : Population(individual_count, 1, max_tree_size, fs) {}
    Population(int individual_count,
               int subpopulation_count,
               int max_tree_size,
               const FunctionSet& fs)
      : Population(individual_count, subpopulation_count, max_tree_size, &fs) {}
    Population(int individual_count,
               int subpopulation_count,
               int max_tree_size,
               const FunctionSet* fs)
    {
        assert(subpopulation_count > 0);
        subpopulations_.resize(subpopulation_count);
        for (int i = 0; i < individual_count; i++)
        {
            Individual* new_individual = ((max_tree_size == 0) ?
                                          new Individual :
                                          new Individual(max_tree_size, *fs));
            subpopulation(i % subpopulation_count).push_back(new_individual);
        }
        updateSortedCollectionOfIndividuals();
        // TODO keep or remove?
        assert(individual_count == sorted_collection_.size());
        assert(individual_count == getIndividualCount());
    }
    
    virtual ~Population()
    {
        applyToAllIndividuals([](Individual* i){ delete i; });
    }

    // A subpopulation (deme): just an std::vector of Individual pointers.
    typedef std::vector<Individual*> SubPop;
    // Functions that implement tournaments, by transforming a TournamentGroup.
    typedef std::function<TournamentGroup(TournamentGroup)> TournamentFunction;
    // Functions that measure "absolute" fitness of an Individual in isolation.
    // (A shortcut for fitnesses that can be measured this way. Many cannot.)
    typedef std::function<float(Individual*)> FitnessFunction;

    // Perform one step of the "steady state" evolutionary computation. Three
    // Individuals are selected randomly, from a random subpopulation. Holds a
    // "tournament" to determine their relative fitness ordering. The "loser" is
    // removed from the Population and replaced by a new "offspring" created by
    // crossing over the two "winners" and mutating the result. Handle migration
    // between subpopulations and maintain sorted index of Individuals.
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        // Choose a random subpopulation, and a random TournamentGroup from it.
        SubPop& subpop = randomSubpopulation();
        TournamentGroup random_group = randomTournamentGroup(subpop);
        // Run tournament amoung the three, return ranked group.
        TournamentGroup ranked_group = tournament_function(random_group);
        Individual* loser = ranked_group.worstIndividual();
        int loser_index = ranked_group.worstIndex();
        assert(loser);
        // Other two become parents of new offspring.
        Individual* parent_0 = ranked_group.secondBestIndividual();
        Individual* parent_1 = ranked_group.bestIndividual();
        // Both parent's rank increases because they survived the tournament.
        parent_0->incrementTournamentsSurvived();
        parent_1->incrementTournamentsSurvived();
        // Create new offspring tree by crossing-over these two parents.
        GpTree new_tree;
        function_set.crossover(parent_0->tree(), parent_1->tree(), new_tree);
        // Mutate constants in new tree.
        new_tree.mutate();
        // Create new offspring Individual from new tree.
        Individual* offspring = new Individual(new_tree);
        // Construct and cache the result of evaluating new offspring's GpTree.
        offspring->treeValue();
        // Delete tournament loser from Population, replace with new offspring.
        replaceIndividual(loser_index, offspring, subpop);
        // Occasionally migrate Individuals between subpopulations.
        subpopulationMigration();
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20210107 replace randomSubpopulation() add unit tests.
//        step_count_++;
        incrementStepCount();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20210105 unit tests for subpopulations, etc.
//        updateSortedCollectionOfIndividuals();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        logger();
    }

    // Perform one step of the "steady state" evolutionary computation using
    // "absolute fitness" (rather than "relative tournament-based fitness").
    // Takes a FitnessFunction which maps a given Individual to a numeric
    // "absolute fitness" value. Converts this into a TournamentFunction for
    // use in the "relative fitness" version of evolutionStep() above.
    void evolutionStep(FitnessFunction fitness_function,
                       const FunctionSet& function_set)
    {
        // Wrap given FitnessFunction to ensure Individual has cached fitness.
        auto augmented_fitness_function = [&](Individual* individual)
        {
            // In case Individual does not already have a cached fitness value.
            if (!(individual->hasFitness()))
            {
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // TODO 20210105 unit tests for subpopulations, etc.
                // The existing sort index, if any, is now invalid.
                sort_cache_invalid_ = true;
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // Tree value should be previously cached, but just to be sure.
                individual->treeValue();
                // Cache fitness on Individual using given FitnessFunction.
                individual->setFitness(fitness_function(individual));
            }
            return individual->getFitness();
        };
        // Create a TournamentFunction based on the augmented FitnessFunction.
        auto tournament_function = [&](TournamentGroup group)
        {
            group.setAllMetrics(augmented_fitness_function);
            return group;
        };
        // Finally do a tournament based evolution step.
        evolutionStep(tournament_function, function_set);
    }
    
    // Delete Individual at index i, then overwrite pointer with replacement.
    void replaceIndividual(int i, Individual* new_individual, SubPop& subpop)
    {
        delete subpop.at(i);
        subpop.at(i) = new_individual;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20210105 unit tests for subpopulations, etc.
        // The existing sort index, if any, is now invalid.
        sort_cache_invalid_ = true;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    
    // TournamentGroup with three Individuals selected randomly from "subpop".
    TournamentGroup randomTournamentGroup(const SubPop& subpop)
    {
        int i = randomIndex(subpop);
        int j = randomIndex(subpop);
        int k = randomIndex(subpop);
        return TournamentGroup({ {subpop.at(i), i},
                                 {subpop.at(j), j},
                                 {subpop.at(k), k} });
    }
    

    // Select a unifromly distributed random index of Population's Individuals.
    int randomIndex(const SubPop& subpop) const
    {
        return LPRS().randomN(subpop.size());
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210105 unit tests for subpopulations, etc.

//        // Return a reference to a randomly selected subpopulation
//        SubPop& randomSubpopulation()
//        {
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//            // TODO 20210105 unit tests for subpopulations, etc.
//
//    //        return subpopulations_.at(LPRS().randomN(subpopulations_.size()));
//
//    //        int random_subpop_index = LPRS().randomN(subpopulations_.size());
//    //        debugPrint(random_subpop_index);
//    //        return subpopulations_.at(random_subpop_index);
//
//            size_t s = subpopulations_.size();
//            int scaled = s * LPRS().random2(0.0f, s);
//            int random_subpop_index = std::min(scaled, 3);
//            debugPrint(random_subpop_index);
//            return subpopulations_.at(random_subpop_index);
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        }

    // TODO 20210105 prototype, try doing simple round robin for SubPop selection
    int next_subpop_ = 0;
    
    // Return a reference to a randomly selected subpopulation
    SubPop& randomSubpopulation()
    {
        int random_subpop_index = (next_subpop_++) % subpopulations_.size();
        debugPrint(random_subpop_index);
        return subpopulations_.at(random_subpop_index);
    }

    
//    // Called each step to update/maintain fitness sorted index of Individuals.
//    // TODO 20201231 this should MERGE the sorted index of each subpopulation.
//    void updateSortedCollectionOfIndividuals()
//    {
//        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//        // TODO 20210101 demes / subpopulations VERY TEMP just for prototyping
//        // // Update cached collection of all Individuals.
//        // sorted_collection_ = individuals();
//        sorted_collection_.clear();
//        applyToAllIndividuals([&]
//                              (Individual* i)
//                              { sorted_collection_.push_back(i); });
//        // TODO should rather merge together presorted lists from each deme.
//        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//        // Sort with largest fitness Individuals at the front.
//        std::sort(sorted_collection_.begin(),
//                  sorted_collection_.end(),
//                  [](Individual* a, Individual* b)
//                  { return a->getFitness() > b->getFitness(); });
//    }
    
//    // Called each step to update/maintain fitness sorted index of Individuals.
//    // TODO 20201231 this should MERGE the sorted index of each subpopulation.
//    void updateSortedCollectionOfIndividuals()
//    {
//        Timer t0("whole updateSortedCollectionOfIndividuals()");
//        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//        // TODO 20210101 demes / subpopulations VERY TEMP just for prototyping
//        // // Update cached collection of all Individuals.
//        // sorted_collection_ = individuals();
//        {
//            Timer t1("collect for updateSortedCollectionOfIndividuals()");
//            sorted_collection_.clear();
//            applyToAllIndividuals([&]
//                                  (Individual* i)
//                                  { sorted_collection_.push_back(i); });
//        }
//        // TODO should rather merge together presorted lists from each deme.
//        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//        // Sort with largest fitness Individuals at the front.
//        {
//            Timer t2("sort for updateSortedCollectionOfIndividuals()");
//            std::sort(sorted_collection_.begin(),
//                      sorted_collection_.end(),
//                      [](Individual* a, Individual* b)
//                      { return a->getFitness() > b->getFitness(); });
//        }
//    }

//        // Called each step to update/maintain fitness sorted index of Individuals.
//        // TODO 20201231 this should MERGE the sorted index of each subpopulation.
//        void updateSortedCollectionOfIndividuals()
//        {
//    //        Timer t0("whole updateSortedCollectionOfIndividuals()");
//    //        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//    //        // TODO 20210101 demes / subpopulations VERY TEMP just for prototyping
//    //        // // Update cached collection of all Individuals.
//    //        // sorted_collection_ = individuals();
//    //        {
//    //            Timer t1("collect for updateSortedCollectionOfIndividuals()");
//                sorted_collection_.clear();
//                applyToAllIndividuals([&]
//                                      (Individual* i)
//                                      { sorted_collection_.push_back(i); });
//    //        }
//    //        // TODO should rather merge together presorted lists from each deme.
//    //        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            // Sort with largest fitness Individuals at the front.
//    //        {
//                Timer t2("sort for updateSortedCollectionOfIndividuals()");
//                std::sort(sorted_collection_.begin(),
//                          sorted_collection_.end(),
//                          [](Individual* a, Individual* b)
//                          { return a->getFitness() > b->getFitness(); });
//    //        }
//        }

    // Called each step to create fitness sorted index of Individual pointers.
    //
    // (NOTE: when I introduced subpopulations (20210103) I made a "quick and
    // dirty" update to this function, leaving nots to come back and speed it
    // up. I was going to do that (on 20210105) but first measured it. For a
    // population of 100 it takes 0.0000376 seconds to run. (Even with a
    // population of 1000 it is about 0.0001 seconds.) Compared to everything
    // else, this seems negligible. If LazyPredator is ever used for much larger
    // population, say 10,000 or more, it is possible this might be an issue.)
    void updateSortedCollectionOfIndividuals()
    {
        if (sort_cache_invalid_)
        {
            // Collect pointers to all Individuals into sorted_collection_.
            sorted_collection_.clear();
            applyToAllIndividuals([&]
                                  (Individual* i)
                                  { sorted_collection_.push_back(i); });
            // Sort with largest fitness Individuals at the front.
            std::sort(sorted_collection_.begin(),
                      sorted_collection_.end(),
                      [](Individual* a, Individual* b)
                      { return a->getFitness() > b->getFitness(); });
        }
        sort_cache_invalid_ = false;
    }

    // TODO 20210103 maybe we want a flag to indicate then the sorted index
    // needs to be re-cached. replaceIndividual() would clear the flag. The
    // bestFitness() functions could call a ensureSorted() which would call
    // updateSortedCollectionOfIndividuals() when needed.
    
//    // Return pointer to Individual with best fitness.
//    Individual* bestFitness() const { return nthBestFitness(0); }
//    // Return pointer to Individual with nth best fitness (0 -> best).
//    Individual* nthBestFitness(int n) const { return sorted_collection_.at(n); }
    
    
    // TODO 20210105 I was thinking this should use a "cache invaid" flag but
    // this is just to protptype where sort is done every time a request is made:
    // oops needed to mark non-const

    // Return pointer to Individual with best fitness.
    Individual* bestFitness() // const
    {
        updateSortedCollectionOfIndividuals();
        return nthBestFitness(0);
    }
    // Return pointer to Individual with nth best fitness (0 -> best).
    Individual* nthBestFitness(int n) // const
    {
        updateSortedCollectionOfIndividuals();
        return sorted_collection_.at(n);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Average of "tree size" over all Individuals.
    int averageTreeSize() const
    {
        int total = 0;
        auto f = [&](Individual* i){ total += i->tree().size(); };
        applyToAllIndividuals(f);
        return total / getIndividualCount();
    }
    
    // Average of "tournaments survived" (or abs fitness) over all Individuals.
    float averageFitness() const
    {
        float total = 0;
        auto f = [&](Individual* i){ total += i->getFitness(); };
        applyToAllIndividuals(f);
        return total / getIndividualCount();
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210105 unit tests for subpopulations, etc.


//    // Occasionally migrate Individuals between subpopulations.
//    // (Swaps two Individuals between "adjacent" subpopulations.)
//    void subpopulationMigration()
//    {
//        // TODO this rate should be adjustable, fixed for this prototype.
//        float subpop_count = subpopulations_.size();
//        float likelihood = subpop_count / getIndividualCount();
//        if ((LPRS().frandom01() < likelihood) && (subpop_count > 1))
//        {
//            // Randomly pick two "adjacent" SubPop. (Reconsider "adjacent".)
//            // Maybe this should be a utility related to randomSubpopulation()?
//            int pm = LPRS().randomBool() ? -1 : 1;
//            int subpop_index_1 = LPRS().randomN(subpopulations_.size());
//            int subpop_index_2 = (subpop_index_1 + pm) % subpopulations_.size();
//            SubPop& subpop1 = subpopulation(subpop_index_1);
//            SubPop& subpop2 = subpopulation(subpop_index_2);
//            // Randomly pick an Individual in each SubPop.
//            int individual_index_1 = randomIndex(subpop1);
//            int individual_index_2 = randomIndex(subpop2);
//            Individual* individual_1 = subpop1.at(individual_index_1);
//            Individual* individual_2 = subpop2.at(individual_index_2);
//            // Swap them.
//            subpop1.at(individual_index_1) = individual_2;
//            subpop2.at(individual_index_2) = individual_1;
//        }
//    }
    
    
    // TODO very temp
    int migrate_consider = 0;
    int migrate_perform = 0;

    // Occasionally migrate Individuals between subpopulations.
    // (Swaps two Individuals between "adjacent" subpopulations.)
    void subpopulationMigration()
    {
        migrate_consider++;  // TODO very temp
        
        // TODO this rate should be adjustable, fixed for this prototype.
        float subpop_count = subpopulations_.size();
        float likelihood = subpop_count / getIndividualCount();
        if ((LPRS().frandom01() < likelihood) && (subpop_count > 1))
        {
            migrate_perform++;  // TODO very temp
            
            // Randomly pick two "adjacent" SubPop. (Reconsider "adjacent".)
            // Maybe this should be a utility related to randomSubpopulation()?
            int pm = LPRS().randomBool() ? -1 : 1;
            int subpop_index_1 = LPRS().randomN(subpopulations_.size());
            int subpop_index_2 = (subpop_index_1 + pm) % subpopulations_.size();
            SubPop& subpop1 = subpopulation(subpop_index_1);
            SubPop& subpop2 = subpopulation(subpop_index_2);
            // Randomly pick an Individual in each SubPop.
            int individual_index_1 = randomIndex(subpop1);
            int individual_index_2 = randomIndex(subpop2);
            Individual* individual_1 = subpop1.at(individual_index_1);
            Individual* individual_2 = subpop2.at(individual_index_2);
            // Swap them.
            subpop1.at(individual_index_1) = individual_2;
            subpop2.at(individual_index_2) = individual_1;
        }
        
        // TODO very temp
        std::cout << "migrate_consider=" << migrate_consider;
        std::cout << " migrate_perform=" << migrate_perform;
        std::cout << " ratio=" << migrate_perform / float(migrate_consider);
        std::cout << std::endl;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Run "steps" of evolution, given "function_set" and "tournament_function".
    void run(int steps,
             const FunctionSet& function_set,
             TournamentFunction tournament_function)
    {
        // Run given number of steps.
        for (int i = 0; i < steps; i++)
        {
            // Run evolution step with given tournament and function set.
            evolutionStep(tournament_function, function_set);
        }
    }
    
    // Called at the end of each evolutionStep(). Can override by subclassing or
    // with setLoggerFunction().
    virtual void logger()
    {
        if (logger_function_) logger_function_(*this);
    }
    void setLoggerFunction(std::function<void(Population&)> logger_function)
    {
        logger_function_ = logger_function;
    }
    static void basicLogger(Population& population)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock>
            now_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>
            elapsed_time = now_time - population.start_time_;
        population.start_time_ = now_time;
        int default_precision = int(std::cout.precision());
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20210107 replace randomSubpopulation() add unit tests.
//        std::cout << population.step_count_ << ": t=";
        std::cout << population.getStepCount() << ": t=";
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        std::cout << std::setprecision(3) << elapsed_time.count() << ", ";
        std::cout << std::setprecision(default_precision);
        std::cout << "pop ave size=" << population.averageTreeSize();
        std::cout << " fit=" << population.averageFitness() << ", ";
        std::cout << "pop best (" << std::setprecision(2);
        for (int i = 0; i < 10; i++)
        {
            if (i > 0) std::cout << " ";
            std::cout << population.nthBestFitness(i)->getFitness();
        }
        std::cout << ")" << std::setprecision(default_precision);
        std::cout << std::endl;
    }
    
    // Apply the given function to all Individuals in this Population.
    void applyToAllIndividuals(std::function<void(Individual*)> func) const
    {
        for (auto& subpop : subpopulations_)
        {
            for (auto& individual : subpop) { func(individual); }
        }
    }
    
    // Return (writable/non-const) reference to i-th subpopulation.
    // Maybe this should be private, perhaps with a const public version?
    SubPop& subpopulation(int s) { return subpopulations_.at(s); }
    
    // Returns total number of Individuals contained in this Population
    int getIndividualCount() const
    {
        int count = 0;
        for (auto& subpop : subpopulations_) { count += subpop.size(); }
        return count;
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210105 unit tests for subpopulations, etc.
    
    // Returns number of subpopulations.
    int getSubpopulationCount() const { return int(subpopulations_.size()); }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210107 replace randomSubpopulation() add unit tests.
    
    // Returns number of evolution steps already taken in this Population.
    int getStepCount() const { return step_count_; }
    void incrementStepCount() { step_count_++; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
    std::function<void(Population&)> logger_function_ = basicLogger;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    int step_count_ = 0;
    // Cached collection of all Individuals sorted by fitness.
    SubPop sorted_collection_;
    // One or more collections of Individual*, each a subpopulation (deme).
    std::vector<SubPop> subpopulations_;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210105 unit tests for subpopulations, etc.
    // TODO should be private
    bool sort_cache_invalid_ = true;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
