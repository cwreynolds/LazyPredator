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
    const std::vector<Individual*>& individuals() const { return individuals_; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void evolutionStep(std::function<Individual*
                                     (Individual*, Individual*, Individual*)>
                       tournament_function,
                       const FunctionSet& function_set)
    {
        auto [i, j, k] = selectThreeIndices();
        Individual* a = individual(i);
        Individual* b = individual(j);
        Individual* c = individual(k);

        Individual* loser = tournament_function(a, b, c);
        
        // TODO this seems awkward, think of a better way:
        int loser_index = k;
        Individual* parent_0 = a;
        Individual* parent_1 = b;
        if (loser == a) { loser_index = i; parent_0 = b; parent_1 = c; }
        if (loser == b) { loser_index = i; parent_0 = a; parent_1 = c; }

        Individual* offspring = new Individual();
        function_set.crossover(parent_0->tree(),
                               parent_1->tree(),
                               offspring->tree());
        offspring->tree().mutate();
        
        // TODO crossover
        // TODO mutation
        // TODO replace in population.
        
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Select three random (but guaranteed to be unique) indices into the
    // population for Individuals to be used in a three way tournament.
    std::tuple<int, int, int> selectThreeIndices()
    {
        int count = static_cast<int>(individuals().size()) - 1;
        assert("fewer than 3 in population" && (count >= 2));
        int count13 = count / 3;
        int count23 = (count * 2) / 3;
        return std::make_tuple(LPRS().random2(0, count13),
                               LPRS().random2(count13 + 1, count23),
                               LPRS().random2(count23 + 1, count));
    };
private:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Individual* individual(int i) { return individuals_.at(i); }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::vector<Individual*> individuals_;
};
