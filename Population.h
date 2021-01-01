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
#include <iomanip>

class TournamentGroup;
class TournamentGroupMember;

// Classes to represent the Individuals participating in a tournament. Each one
// is a TournamentGroupMember composed of an Individual pointer, an index in the
// Population, and an optional float fitness metric. The collection of those, a
// TournamentGroup, is passed around functions related to tournaments. The
// members of a group are kept in sorted order.
// TODO maybe move to their own file rather than be distracting clutter here?
class TournamentGroupMember
{
public:
    TournamentGroupMember()
        : individual(nullptr), index(0), metric(0) {}
    TournamentGroupMember(Individual* individual_, int index_)
        : individual(individual_), index(index_), metric(0) {}
    TournamentGroupMember(Individual* individual_, int index_, float metric_)
        : individual(individual_), index(index_), metric(metric_) {}
    Individual* individual = nullptr;  // pointer to an Individual.
    int index = 0;                     // Individual's index in Population.
    float metric = 0;                  // Optional fitness metric.
};

class TournamentGroup
{
public:
    TournamentGroup() { sort(); }
    TournamentGroup(const std::vector<TournamentGroupMember>& member_list)
        : members_(member_list) { sort(); }
    // Const reference to vector of members. (TODO any need for this?)
    const std::vector<TournamentGroupMember>& members() const {return members_;}
    // Number of members in this group (normally 3).
    size_t size() const { return members().size(); }
    // For "numerical fitness"-based tournaments, map a given scoring function
    // over all members to set the metric values. Sorts members afterward.
    void setAllMetrics(std::function<float(Individual*)> scoring)
    {
        for (auto& m : members_) { m.metric = scoring(m.individual); }
        sort();
    }
    int worstIndex() const { return members().front().index; }
    Individual* worstIndividual() const { return members().front().individual; }
    Individual* bestIndividual() const { return members().back().individual; }
    Individual* secondBestIndividual() const
    {
        return members().at(size() - 2).individual;
    }
    // For debugging/testing. TODO or should this be to_string() ?
    void print() const
    {
        // TODO need a template utility for printing an std::vector as a comma
        //      separated list.
        std::cout << "TournamentGroup: {";
        bool first = true;
        for (auto& m : members())
        {
            if (!first) { std::cout << ", "; first = false; }
            std::cout << "{" << m.individual << ", ";
            std::cout << m.index << ", " << m.metric << "}";
        }
        std::cout << "}" << std::endl;
    }
    // Find largest tree size of all Individuals in this TournamentGroup.
    int maxTreeSize() const
    {
        int max_tree_size = std::numeric_limits<int>::min();
        for (auto& m : members())
        {
            int tree_size = m.individual->tree().size();
            if (max_tree_size < tree_size) max_tree_size = tree_size;
        }
        return max_tree_size;
    }
    // TODO maybe this is better suited to needs of CWE?
    // Find smallest tree size of all Individuals in this TournamentGroup.
    int minTreeSize() const
    {
        int min_tree_size = std::numeric_limits<int>::max();
        for (auto& m : members())
        {
            int tree_size = m.individual->tree().size();
            if (min_tree_size > tree_size) min_tree_size = tree_size;
        }
        return min_tree_size;
    }
    // Given an Individual, returns an int from 1 to members().size(),
    // rank 1 corresponds to bestIndividual().
    int rankOfIndividual(Individual* individual)
    {
        int rank = 0;
        int count = int(members().size());
        for (int i = 0; i <  count; i++)
        {
            if (individual == members().at(i).individual) rank = count - i;
        }
        assert("given Individual not in TournamentGroup" && rank != 0);
        return rank;
    }
private:
    // Sort the members of this group by their "metric" value.
    void sort()
    {
        auto sorted = [](const TournamentGroupMember &a,
                         const TournamentGroupMember &b)
                        { return a.metric < b.metric; };
        std::sort(members_.begin(), members_.end(), sorted);
    }
    std::vector<TournamentGroupMember> members_;
};

class Population
{
public:
    Population() {}
    Population(int size)
    {
        for (int i = 0; i < size; i++) individuals_.push_back(new Individual);
        updateSortedCollectionOfIndividuals();
    }
    // TODO combine constructors with and without fs?
    Population(int size, int max_tree_size, const FunctionSet& fs)
    {
        for (int i = 0; i < size; i++)
        {
            individuals_.push_back(new Individual(max_tree_size, fs));
        }
        updateSortedCollectionOfIndividuals();
    }
    virtual ~Population()
    {
        for (Individual* individual : individuals_) { delete individual; }
    }
    // Functions that implement tournaments, by transforming a TournamentGroup.
    typedef std::function<TournamentGroup(TournamentGroup)> TournamentFunction;
    // Return const reference to collection of Individuals in Population.
    const std::vector<Individual*>& individuals() const { return individuals_; }

    // Perform one step of the "steady state" evolutionary computation. Hold a
    // tournament with three randomly selected Individuals. The "loser" is
    // replaced in the Population by a new "offspring" created by crossing over
    // the two "winners" and mutating the result
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        TournamentGroup random_group = randomTournamentGroup();
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
        replaceIndividual(loser_index, offspring);
        // TODO TEMP for debugging
        // TODO 20201122 remove old unused hack
        step_count_++;
        updateSortedCollectionOfIndividuals();
        logger();
    }
    
    // Type for "fitness function" used below.
    typedef std::function<float(Individual*)> FitnessFunction;
    // Perform one step of the "steady state" evolutionary computation using
    // "absolute fitness" (rather than "relative tournament-based fitness").
    // Takes a FitnessFunction which maps a given Individual to a numeric
    // "absolute fitness" value. Converts this into a TournamentFunction for
    // use in the "relative fitness" version of evolutionStep().
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
    
    // Delete Individual at index i, then overwrite pointer with replacement.
    void replaceIndividual(int i, Individual* new_individual)
    {
        delete individuals_.at(i);
        individuals_.at(i) = new_individual;
    }
    // TournamentGroup with three Individuals selected randomly from Population.
    TournamentGroup randomTournamentGroup()
    {
        int i = randomIndex();
        int j = randomIndex();
        int k = randomIndex();
        return TournamentGroup({ {individual(i), i},
                                 {individual(j), j},
                                 {individual(k), k} });
    }

    // Select a unifromly distributed random index of Population's Individuals.
    int randomIndex() const { return LPRS().randomN(individuals().size()); }
    
    // Called each step to update/maintain fitness sorted list of Individuals.
    // TODO 20201231 once we have demes they need to be merged here.
    void updateSortedCollectionOfIndividuals()
    {
        // Update cached collection of all Individuals.
        sorted_collection_ = individuals();
        // Sort with largest fitness Individuals at the front.
        std::sort(sorted_collection_.begin(),
                  sorted_collection_.end(),
                  [](Individual* a, Individual* b)
                  { return a->getFitness() > b->getFitness(); });
    }
    
    // Return pointer to Individual with best fitness.
    Individual* bestFitness() const { return nthBestFitness(0); }
    // Return pointer to Individual with nth best fitness (0 -> best).
    Individual* nthBestFitness(int n) const { return sorted_collection_.at(n); }

    // Average of "tree size" over all Individuals.
    int averageTreeSize() const
    {
        int total = 0;
        for (auto& i : individuals()) { total += i->tree().size(); }
        return total / individuals().size();
    }

    // Average of "tournaments survived" over all Individuals.
    float averageFitness() const
    {
        float total = 0;
        for (auto& i : individuals()) { total += i->getFitness(); }
        return total / individuals().size();
    }

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

    // This utility allows modification of Individuals inside a Population. It
    // is currently used only by UnitTests, to get around "const" protection of
    // Individuals inside a Population. Think hard before using it elsewhere.
    void applyToIndividual(int i, std::function<void(Individual*)> func)
    {
        func(individual(i));
    }
private:
    Individual* individual(int i) { return individuals_.at(i); }
    std::vector<Individual*> individuals_;
    std::function<void(Population&)> logger_function_ = basicLogger;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    int step_count_ = 0;
    // Cached collection of all Individuals sorted by fitness.
    std::vector<Individual*> sorted_collection_;
};
