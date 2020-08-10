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
private:
    std::vector<Individual*> individuals;
};
