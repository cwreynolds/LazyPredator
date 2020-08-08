//
//  main.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/5/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"

int main(int argc, const char * argv[])
{
    UnitTests::allTestsOK();
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Testing instance counter for Individual.
    std::cout << "August 7, 2020" << std::endl;
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200807_";
    
    {
        Individual i1;
        {
            Individual i2;
            {
                Individual i3;
                Population p;
            }
        }
    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
