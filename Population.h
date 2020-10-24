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

class Population
{
public:
    Population() {}
    Population(int size)
    {
        for (int i = 0; i < size; i++) individuals_.push_back(new Individual);
    }
    // TODO combine constructors with and without fs?
    Population(int size, int max_tree_size, const FunctionSet& fs)
    {
        for (int i = 0; i < size; i++)
        {
            individuals_.push_back(new Individual(max_tree_size, fs));
        }
    }
    ~Population()
    {
        while (!individuals_.empty())
        {
            Individual* last = individuals_.back();
            individuals_.pop_back();
            delete last;
        }
    }
    // Type for functions that implement 3-way tournaments, returning the loser.
    typedef std::function<Individual*(Individual*, Individual*, Individual*)>
            TournamentFunction;
    // Return const reference to collection of Individuals in Population.
    const std::vector<Individual*>& individuals() const { return individuals_; }
    // Perform one step of the "steady state" evolutionary computation. Hold a
    // tournament with three randomly selected Individuals. The "loser" is
    // replaced in the Population by a new "offspring" created by crossing over
    // the two "winners" and mutating the result
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        // Select three Individual uniformly distributed across this Population.
        auto [i, j, k] = selectThreeIndices();
        Individual* a = individual(i);
        Individual* b = individual(j);
        Individual* c = individual(k);
        // Run tournament amoung the three, determined which one is the worst.
        Individual* loser = tournament_function(a, b, c);
        // Determine the other two which become parents of new offspring.
        // TODO this seems awkward, think of a better way:
        int loser_index = k;
        Individual* parent_0 = a;
        Individual* parent_1 = b;
        if (loser == a) { loser_index = i; parent_0 = b; parent_1 = c; }
        if (loser == b) { loser_index = i; parent_0 = a; parent_1 = c; }
        // Both parents increase their rank because they survived the tournament
        parent_0->incrementTournamentsSurvived();
        parent_1->incrementTournamentsSurvived();
        // Create new offspring tree from crossover between two parents.
        GpTree offspring_tree;
        function_set.crossover(parent_0->tree(),
                               parent_1->tree(),
                               offspring_tree);
        // Mutate constants in new tree.
        offspring_tree.mutate();
        // Create new offspring Individual from new tree.
        Individual* offspring = new Individual(offspring_tree);
        // Delete tournament loser from Population, replace with new offspring.
        replaceIndividual(loser_index, offspring);
        // TODO TEMP for debugging
        last_individual_added = offspring;
    }

    // TODO TEMP for debugging
    static inline Individual* last_individual_added = nullptr;
    
    // Delete Individual at index i, then overwrite pointer with replacement.
    // TODO merge this with code now in ~Population()
    void replaceIndividual(int i, Individual* new_individual)
    {
        delete individuals_.at(i);
        individuals_.at(i) = new_individual;
    }
    // Select three random (but guaranteed to be unique) indices into the
    // population for Individuals to be used in a three way tournament.
    std::tuple<int, int, int> selectThreeIndices()
    {
        int count = static_cast<int>(individuals().size()) - 1;
        assert("fewer than 3 in population" && (count >= 2));
        int one_third = count / 3;
        int two_thirds = (count * 2) / 3;
        return std::make_tuple(LPRS().random2(0, one_third),
                               LPRS().random2(one_third + 1, two_thirds),
                               LPRS().random2(two_thirds + 1, count));
    };
    // Find the best Individual in Population, defined as having survived the
    // most tournaments. TODO name?
    Individual* findBestIndividual() const
    {
        assert(!individuals().empty()); // Or just return nullptr in this case?
        int most_survived = std::numeric_limits<int>::min();
        Individual* best_individual = nullptr;
        for (auto& individual : individuals())
        {
            int survived = individual->getTournamentsSurvived();
            if (most_survived < survived)
            {
                most_survived = survived;
                best_individual = individual;
            }
        }
        return best_individual;
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
private:
    Individual* individual(int i) { return individuals_.at(i); }
    std::vector<Individual*> individuals_;
};
