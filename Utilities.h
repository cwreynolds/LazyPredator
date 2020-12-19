//
//  Utilities.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/7/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once

#include <map>
#include <any>
#include <set>
#include <limits>

// TODO UTILITIES_NAMESPACES
// TODO temporarily share utilities with TexSyn
#include "../TexSyn/TexSyn.h"
 
// TODO -- LPRS() temporary LazyPredator global RandomSequence used for things
//         like makeRandomTree, crossover, ephemeral generators.  Redesign?
class LP
{
public:
    static RandomSequence& randomSequence() { return random_sequence; }
private:
    static inline RandomSequence random_sequence;
};
inline RandomSequence& LPRS() { return LP::randomSequence(); }
