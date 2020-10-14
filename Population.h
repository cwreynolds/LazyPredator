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
        for (int i = 0; i < size; i++) individuals.push_back(new Individual);
    }
    ~Population()
    {
        while (!individuals.empty())
        {
            Individual* last = individuals.back();
            individuals.pop_back();
            delete last;
        }
    }
    // Select three random (but guaranteed to be unique) indices into the
    // population for Individuals to be used in a three way tournament.
    std::tuple<int, int, int> selectThreeIndices()
    {
        int count = static_cast<int>(individuals.size()) - 1;
        assert("fewer than 3 in population" && (count >= 2));
        int count13 = count / 3;
        int count23 = (count * 2) / 3;
        return std::make_tuple(LPRS().random2(0, count13),
                               LPRS().random2(count13 + 1, count23),
                               LPRS().random2(count23 + 1, count));
    };
private:
    std::vector<Individual*> individuals;
};
