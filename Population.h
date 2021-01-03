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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations

//    Population() {}
//    Population(int size)
//    {
//        for (int i = 0; i < size; i++) individuals_.push_back(new Individual);
//        updateSortedCollectionOfIndividuals();
//    }
//    // TODO combine constructors with and without fs?
//    Population(int size, int max_tree_size, const FunctionSet& fs)
//    {
//        for (int i = 0; i < size; i++)
//        {
//            individuals_.push_back(new Individual(max_tree_size, fs));
//        }
//        updateSortedCollectionOfIndividuals();
//    }
    
    // TODO new prototype

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
//            individuals_.push_back(new_individual); // TODO to be removed
            subpopulation(i % subpopulation_count).push_back(new_individual);
        }
        updateSortedCollectionOfIndividuals();
        
        
        assert(individual_count == sorted_collection_.size());
        assert(individual_count == getIndividualCount());

    }
    
//    virtual ~Population()
//    {
//        for (Individual* individual : individuals_) { delete individual; }
//    }

    virtual ~Population()
    {
        applyToAllIndividuals([](Individual* i){ delete i; });
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations
    // TODO move elsewhere in this file if kept
    // std::vector<std::vector<Individual*>> subpopulations_;
    
    typedef std::vector<Individual*> SubPop;
    
    // Return (writable/non-const) reference to i-th subpopulation.
    // Maybe this should be private, perhaps with a const public version?
//    std::vector<Individual*>& subpopulation(int s)
    SubPop& subpopulation(int s)
    {
        return subpopulations_.at(s);
    }
    
    // Returns total number of Individuals contained in this Population
//    int size() const
//    int individualCount() const
//    int countOfIndividuals() const
    int getIndividualCount() const
    {
        int count = 0;
        for (auto& subpop : subpopulations_) { count += subpop.size(); }
        return count;
    }
        
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Functions that implement tournaments, by transforming a TournamentGroup.
    typedef std::function<TournamentGroup(TournamentGroup)> TournamentFunction;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations
    // Return const reference to collection of Individuals in Population.
//    const std::vector<Individual*>& individuals() const { return individuals_; }
//    const std::vector<Individual*>& delete_me_individuals() const
//        { return individuals_; }
//    const SubPop& delete_me_individuals() const { return individuals_; }

//    // Perform one step of the "steady state" evolutionary computation. Hold a
//    // tournament with three randomly selected Individuals. The "loser" is
//    // replaced in the Population by a new "offspring" created by crossing over
//    // the two "winners" and mutating the result
//    void evolutionStep(TournamentFunction tournament_function,
//                       const FunctionSet& function_set)
//    {
//        TournamentGroup random_group = randomTournamentGroup();
//        // Run tournament amoung the three, return ranked group.
//        TournamentGroup ranked_group = tournament_function(random_group);
//        Individual* loser = ranked_group.worstIndividual();
//        int loser_index = ranked_group.worstIndex();
//        assert(loser);
//        // Other two become parents of new offspring.
//        Individual* parent_0 = ranked_group.secondBestIndividual();
//        Individual* parent_1 = ranked_group.bestIndividual();
//        // Both parent's rank increases because they survived the tournament.
//        parent_0->incrementTournamentsSurvived();
//        parent_1->incrementTournamentsSurvived();
//        // Create new offspring tree by crossing-over these two parents.
//        GpTree new_tree;
//        function_set.crossover(parent_0->tree(), parent_1->tree(), new_tree);
//        // Mutate constants in new tree.
//        new_tree.mutate();
//        // Create new offspring Individual from new tree.
//        Individual* offspring = new Individual(new_tree);
//        // Construct and cache the result of evaluating new offspring's GpTree.
//        offspring->treeValue();
//        // Delete tournament loser from Population, replace with new offspring.
//        replaceIndividual(loser_index, offspring);
//        // TODO TEMP for debugging
//        // TODO 20201122 remove old unused hack
//        step_count_++;
//        updateSortedCollectionOfIndividuals();
//        logger();
//    }

    // Perform one step of the "steady state" evolutionary computation. Hold a
    // tournament with three randomly selected Individuals. The "loser" is
    // replaced in the Population by a new "offspring" created by crossing over
    // the two "winners" and mutating the result
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        // Choose a random subpopulation, and a random TournamentGroup from it.
//        std::vector<Individual*>& subpopulation = randomSubpopulation();
        SubPop& subpop = randomSubpopulation();
//        TournamentGroup random_group = randomTournamentGroup();
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
//        replaceIndividual(loser_index, offspring);
        replaceIndividual(loser_index, offspring, subpop);

        step_count_++;
        updateSortedCollectionOfIndividuals();
        logger();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Type for "fitness function" used below.
    typedef std::function<float(Individual*)> FitnessFunction;
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
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations

//    // Delete Individual at index i, then overwrite pointer with replacement.
//    void replaceIndividual(int i, Individual* new_individual)
//    {
//        delete individuals_.at(i);
//        individuals_.at(i) = new_individual;
//    }

    // Delete Individual at index i, then overwrite pointer with replacement.
    void replaceIndividual(int i, Individual* new_individual, SubPop subpop)
    {
//        delete individuals_.at(i);
//        individuals_.at(i) = new_individual;
        
        std::cout << "replaceIndividual() ";
        debugPrint(&subpop);
        std::cout << "replaceIndividual() i = " << i << std::endl;
        assert(&subpop == &(subpopulations_.at(0)));
        assert(&subpop == &(subpopulation(0)));


        delete subpop.at(i);
        subpop.at(i) = new_individual;
    }
    

//    // TournamentGroup with three Individuals selected randomly from Population.
//    TournamentGroup randomTournamentGroup()
//    {
//        int i = randomIndex();
//        int j = randomIndex();
//        int k = randomIndex();
//        return TournamentGroup({ {individual(i), i},
//                                 {individual(j), j},
//                                 {individual(k), k} });
//    }
    
    // TournamentGroup with three Individuals selected randomly from Population.
//    TournamentGroup randomTournamentGroup(const std::vector<Individual*>& individuals)
    TournamentGroup randomTournamentGroup(const SubPop& subpop)
    {
        int i = randomIndex(subpop);
        int j = randomIndex(subpop);
        int k = randomIndex(subpop);
        
        std::cout << "randomTournamentGroup() ";
        debugPrint(&subpop);
        assert(&subpop == &subpopulation(0));

        std::cout << "randomTournamentGroup() i,j,k = "
                  << i << ", " << j << ", " << k << std::endl;
        
        return TournamentGroup({ {subpop.at(i), i},
                                 {subpop.at(j), j},
                                 {subpop.at(k), k} });
    }
    

    // Select a unifromly distributed random index of Population's Individuals.
//    int randomIndex() const { return LPRS().randomN(individuals().size()); }
//    int randomIndex(const std::vector<Individual*>& individuals) const
    int randomIndex(const SubPop& subpop) const
    {
        return LPRS().randomN(subpop.size());
    }
    
    
//    std::vector<Individual*>& randomSubpopulation() const
//    std::vector<Individual*>& randomSubpopulation()
    SubPop& randomSubpopulation()
    {
//        return subpopulations_.at(LPRS().randomN(subpopulations_.size()));

        int n = LPRS().randomN(subpopulations_.size());
        
        auto& subpop = subpopulations_.at(n);
        std::cout << "randomSubpopulation() ";
        debugPrint(&subpop);
        assert(&subpop == &subpopulation(0));

        std::cout << "randomSubpopulation() n = " << n << std::endl;
        return subpopulations_.at(n);

    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations
    
//    // Called each step to update/maintain fitness sorted list of Individuals.
//    // TODO 20201231 once we have demes they need to be merged here.
//    void updateSortedCollectionOfIndividuals()
//    {
//        // Update cached collection of all Individuals.
//        sorted_collection_ = individuals();
//        // Sort with largest fitness Individuals at the front.
//        std::sort(sorted_collection_.begin(),
//                  sorted_collection_.end(),
//                  [](Individual* a, Individual* b)
//                  { return a->getFitness() > b->getFitness(); });
//    }

    // Called each step to update/maintain fitness sorted list of Individuals.
    // TODO 20201231 once we have demes they need to be merged here.
    void updateSortedCollectionOfIndividuals()
    {
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        // TODO 20210101 demes / subpopulations VERY TEMP just for prototyping
        
//        // Update cached collection of all Individuals.
//        sorted_collection_ = individuals();

        sorted_collection_.clear();
        applyToAllIndividuals([&]
                              (Individual* i)
                              { sorted_collection_.push_back(i); });
        
        // TODO should rather merge together presorted lists from each deme.
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        // Sort with largest fitness Individuals at the front.
        std::sort(sorted_collection_.begin(),
                  sorted_collection_.end(),
                  [](Individual* a, Individual* b)
                  { return a->getFitness() > b->getFitness(); });
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Return pointer to Individual with best fitness.
    Individual* bestFitness() const { return nthBestFitness(0); }
    // Return pointer to Individual with nth best fitness (0 -> best).
    Individual* nthBestFitness(int n) const { return sorted_collection_.at(n); }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations

//    // Average of "tree size" over all Individuals.
//    int averageTreeSize() const
//    {
//        int total = 0;
//        for (auto& i : individuals()) { total += i->tree().size(); }
//        return total / individuals().size();
//    }
//
//    // Average of "tournaments survived" over all Individuals.
//    float averageFitness() const
//    {
//        float total = 0;
//        for (auto& i : individuals()) { total += i->getFitness(); }
//        return total / individuals().size();
//    }

    // Average over all Individuals of "tree size".
    int averageTreeSize() const
    {
        int total = 0;
//        auto f = [&](Individual* i){ total += i->tree().size(); };
        auto f = [&](Individual* i)
        {
//            i->treeValue();
            assert(i);
            total += i->tree().size();
        };
        applyToAllIndividuals(f);
        return total / getIndividualCount();
    }
    
    // Average over all Individuals of "tournaments survived" (or abs fitness).
    float averageFitness() const
    {
        int total = 0;
        auto f = [&](Individual* i){ total += i->getFitness(); };
        applyToAllIndividuals(f);
        return total / getIndividualCount();
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
        std::cout << population.step_count_ << ": t=";
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations

//    // This utility allows modification of Individuals inside a Population. It
//    // is currently used only by UnitTests, to get around "const" protection of
//    // Individuals inside a Population. Think hard before using it elsewhere.
//    void applyToIndividual(int i, std::function<void(Individual*)> func)
//    {
//        func(individual(i));
//    }
    
    // Apply the given function to all Individuals in this Population.
    
    void applyToAllIndividuals(std::function<void(Individual*)> func) const
    {
        for (auto& subpop : subpopulations_)
        {
            for (auto& individual : subpop) { func(individual); }
//            for (auto& individual : subpop)
//            {
//                assert(individual);
//                assert(individual->tree().getRootValue().has_value());
//                func(individual);
//
//            }
        }
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
//    Individual* individual(int i) { return individuals_.at(i); }
//    std::vector<Individual*> individuals_;
    std::function<void(Population&)> logger_function_ = basicLogger;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    int step_count_ = 0;
    // Cached collection of all Individuals sorted by fitness.
//    std::vector<Individual*> sorted_collection_;
    SubPop sorted_collection_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 20210101 demes / subpopulations
    
    // One or more collections of Individual*, each a subpopulation (deme).
//    std::vector<std::vector<Individual*>> subpopulations_;
    std::vector<SubPop> subpopulations_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
