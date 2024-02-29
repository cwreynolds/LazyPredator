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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20240229 fixes in LazyPredator and TexSyn while debugging evoflock.
//#include "../TexSyn/TexSyn.h"
#include "../TexSyn/Utilities.h"
#include "../TexSyn/RandomSequence.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 
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
