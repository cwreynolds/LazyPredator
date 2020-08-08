//
//  Individual.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"

class Individual
{
public:
    Individual()
    {
        getSetInstanceCount()++;
    }
    ~Individual()
    {
        getSetInstanceCount()--;
    }
    static const int& getInstanceCount() { return getSetInstanceCount(); }
private:
    static int& getSetInstanceCount() { static int count = 0;  return count; }
};
