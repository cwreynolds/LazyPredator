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
    Population() : Population(0, 0, 0, 0, 0, nullptr) {}
    Population(int individual_count)
      : Population(individual_count, 0, 0, 0, 0, nullptr) {}
    Population(int individual_count,
               int max_init_tree_size,
               const FunctionSet& fs)
      : Population(individual_count, 0, max_init_tree_size, fs) {}
    Population(int individual_count,
               int subpopulation_count,
               int max_init_tree_size,
               const FunctionSet& fs)
      : Population(individual_count,
                   subpopulation_count,
                   max_init_tree_size,
                   0.5 * max_init_tree_size,
                   1.5 * max_init_tree_size,
                   &fs) {}
    Population(int individual_count,
               int subpopulation_count,
               int max_init_tree_size,
               int min_crossover_tree_size,
               int max_crossover_tree_size,
               const FunctionSet& fs)
      : Population(individual_count,
                   subpopulation_count,
                   max_init_tree_size,
                   min_crossover_tree_size,
                   max_crossover_tree_size,
                   &fs) {}
    Population(int individual_count,
               int subpopulation_count,
               int max_init_tree_size,
               int min_crossover_tree_size,
               int max_crossover_tree_size,
               const FunctionSet* fs)
    {
        setFunctionSet(fs);
        setMaxInitTreeSize(max_init_tree_size);
        setMinCrossoverTreeSize(min_crossover_tree_size);
        setMaxCrossoverTreeSize(max_crossover_tree_size);
        if (subpopulation_count == 0) { subpopulation_count = 1; } // Default.
        assert(subpopulation_count > 0);
        subpopulations_.resize(subpopulation_count);
        for (int i = 0; i < individual_count; i++)
        {
            Individual* new_individual = ((max_init_tree_size == 0) ?
                                          new Individual :
                                          new Individual(max_init_tree_size,
                                                         *fs));
            subpopulation(i % subpopulation_count).push_back(new_individual);
        }
        updateSortedCollectionOfIndividuals();
        // TODO keep, remove, or move to unit tests?
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
    void evolutionStep(TournamentFunction tournament_function)
    {
        // Get current subpopulation, create a random TournamentGroup from it.
        SubPop& subpop = currentSubpopulation();
        TournamentGroup random_group = randomTournamentGroup(subpop);
        // Run tournament amoung the three, return ranked group.
        TournamentGroup ranked_group = tournament_function(random_group);
        Individual* loser = ranked_group.worstIndividual();
        int loser_index = ranked_group.worstIndex();
        assert(loser);
        // Other two become parents of new offspring.
        Individual* parent0 = ranked_group.secondBestIndividual();
        Individual* parent1 = ranked_group.bestIndividual();
        // Both parent's rank increases because they survived the tournament.
        parent0->incrementTournamentsSurvived();
        parent1->incrementTournamentsSurvived();
        // Create new offspring tree by crossing-over these two parents.
        GpTree new_tree;
        GpTree::crossover(parent0->tree(),
                          parent1->tree(),
                          new_tree,
                          getMinCrossoverTreeSize(),
                          getMaxCrossoverTreeSize(),
                          getFunctionSet()->getCrossoverMinSize());
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
        // Increment step count (before logger() call for 1 based step numbers).
        incrementStepCount();
        logger();
    }

    // Perform one step of the "steady state" evolutionary computation using
    // "absolute fitness" (rather than "relative tournament-based fitness").
    // Takes a FitnessFunction which maps a given Individual to a numeric
    // "absolute fitness" value. Converts this into a TournamentFunction for
    // use in the "relative fitness" version of evolutionStep() above.
    void evolutionStep(FitnessFunction fitness_function)
    {
        // Wrap given FitnessFunction to ensure Individual has cached fitness.
        auto augmented_fitness_function = [&](Individual* individual)
        {
            // In case Individual does not already have a cached fitness value.
            if (!(individual->hasFitness()))
            {
                // The existing sort index, if any, is now invalid.
                sort_cache_invalid_ = true;
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
        // Finally, do a tournament-based evolution step.
        evolutionStep(tournament_function);
    }
    
    // Delete Individual at index i, then overwrite pointer with replacement.
    void replaceIndividual(int i, Individual* new_individual, SubPop& subpop)
    {
        delete subpop.at(i);
        subpop.at(i) = new_individual;
        sort_cache_invalid_ = true;
    }
    
    // TournamentGroup with three Individuals selected randomly from "subpop".
    TournamentGroup randomTournamentGroup(const SubPop& subpop)
    {
        auto [i, j, k] = threeUniqueRandomIndices(subpop);
        return TournamentGroup({ {subpop.at(i), i},
                                 {subpop.at(j), j},
                                 {subpop.at(k), k} });
    }

    // Select three unique random indices of a SubPop's Individuals.
    std::tuple<int,int,int> threeUniqueRandomIndices(const SubPop& subpop) const
    {
        assert(subpop.size() >= 3);
        std::vector<int> i;  // Vec to hold the three unique indices.
        int max_tries = 1000;
        auto add_unique_index = [&]()
        {
            int index = randomIndividualIndex(subpop);
            while (std::find(i.begin(), i.end(), index) != i.end())
            {
                index = randomIndividualIndex(subpop);
                assert(max_tries-- > 0); // Should never happen but I'm paranoid.
            }
            i.push_back(index);
        };
        add_unique_index();
        add_unique_index();
        add_unique_index();
        assert((i.at(0)!=i.at(1)) || (i.at(1)!=i.at(2)) || (i.at(2)!=i.at(0)));
        return std::make_tuple(i.at(0), i.at(1), i.at(2));
    }
    
    // Select a uniformly distributed random index of a Subpop's Individuals.
    int randomIndividualIndex(const SubPop& subpop) const
    {
        return LPRS().randomN(subpop.size());
    }

    // Return index of the subpopulation to be updated this step.
    int currentSubpopulationIndex() const
    {
        return getStepCount() % getSubpopulationCount();
    }
        
    // Return a reference to the subpopulation to be updated this step.
    SubPop& currentSubpopulation()
    {
        return subpopulations_.at(currentSubpopulationIndex());
    }

    // Called each step to create fitness sorted index of Individual pointers.
    //
    // (NOTE: when I introduced subpopulations (20210103) I made a "quick and
    // dirty" update to this function, leaving notes to come back and speed it
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
    
    // Return pointer to Individual with best fitness.
    Individual* bestFitness()
    {
        updateSortedCollectionOfIndividuals();
        return nthBestFitness(0);
    }
    // Return pointer to Individual with nth best fitness (0 -> best).
    Individual* nthBestFitness(int n)
    {
        updateSortedCollectionOfIndividuals();
        return sorted_collection_.at(n);
    }

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
    
    // Occasionally migrate (swap) Individuals between current and random SubPop.
    void subpopulationMigration()
    {
        int spc = getSubpopulationCount();
        if ((spc > 1) && (LPRS().frandom01() < getMigrationLikelihood()))
        {
            // Get indices of, and references to, the current subpopulation,
            // plus a different, random subpopulation.
            int random_index_offset = 1 + LPRS().randomN(spc - 1);
            int subpop_index_1 = currentSubpopulationIndex();
            int subpop_index_2 = (subpop_index_1 + random_index_offset) % spc;
            SubPop& subpop1 = subpopulation(subpop_index_1);
            SubPop& subpop2 = subpopulation(subpop_index_2);
            // Randomly pick an Individual in each SubPop.
            int individual_index_1 = randomIndividualIndex(subpop1);
            int individual_index_2 = randomIndividualIndex(subpop2);
            Individual* individual_1 = subpop1.at(individual_index_1);
            Individual* individual_2 = subpop2.at(individual_index_2);
            // Swap them.
            subpop1.at(individual_index_1) = individual_2;
            subpop2.at(individual_index_2) = individual_1;
        }
    }

    // Run "steps" of evolution, given "tournament_function".
    void run(int steps, TournamentFunction tournament_function)
    {
        // Run given number of steps.
        for (int i = 0; i < steps; i++)
        {
            // Run evolution step with given tournament and function set.
            evolutionStep(tournament_function);
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
        std::cout << population.getStepCount() << ": t=";
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
    
    // Get/set this Population's FunctionSet.
    const FunctionSet* getFunctionSet() const { return function_set_; }
    void setFunctionSet(const FunctionSet* fs) { function_set_ = fs; }

    // Returns number of subpopulations.
    int getSubpopulationCount() const { return int(subpopulations_.size()); }
    // Returns number of evolution steps already taken in this Population.
    int getStepCount() const { return step_count_; }
    void incrementStepCount() { step_count_++; }
    // The probability, on any given evolutionStep(), that migration will occur.
    float getMigrationLikelihood() const { return migration_likelihood_; }
    void setMigrationLikelihood(float ml) { migration_likelihood_ = ml; }
    
    // Get/set this Population's max tree size for initial random trees.
    int getMaxInitTreeSize() const { return max_init_tree_size_; }
    void setMaxInitTreeSize(int size) { max_init_tree_size_ = size; }
    
    // Get/set min/max crossover tree size.
    int getMinCrossoverTreeSize() const { return min_crossover_tree_size_; }
    void setMinCrossoverTreeSize(int size) { min_crossover_tree_size_ = size; }
    int getMaxCrossoverTreeSize() const { return max_crossover_tree_size_; }
    void setMaxCrossoverTreeSize(int size) { max_crossover_tree_size_ = size; }

private:
    std::function<void(Population&)> logger_function_ = basicLogger;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    int step_count_ = 0;
    // One or more collections of Individual*, each a subpopulation (deme).
    std::vector<SubPop> subpopulations_;
    // Cached index of all Individuals sorted by fitness.
    SubPop sorted_collection_;
    // Sorted index of Individuals is cached until a change is made.
    bool sort_cache_invalid_ = true;
    // Const pointer to this Population's FunctionSet.
    const FunctionSet* function_set_ = nullptr;
    // The probability, on any given evolutionStep(), that migration will occur.
    float migration_likelihood_ = 0.05;
    // Max size for initial random trees.
    int max_init_tree_size_ = 0;
    // Min/max crossover tree size.
    int min_crossover_tree_size_ = 0;
    int max_crossover_tree_size_ = std::numeric_limits<int>::max();
};
