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
//    // Testing instance counter for Individual.
//    std::cout << "August 7, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200807_";
//
//    {
//        Individual i1;
//        {
//            Individual i2;
//            {
//                Individual i3;
//                Population p;
//            }
//        }
//    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Testing FunctionSet.
    std::cout << "August 9, 2020" << std::endl;
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200809_";
    FunctionSet fs;
    
    fs.addType("A");
    fs.addType("B");
    fs.addType("C");
    
    debugPrint(fs.typeIdFromName("A"));
    debugPrint(fs.typeIdFromName("B"));
    debugPrint(fs.typeIdFromName("C"));
//    debugPrint(fs.typeIdFromName("D"));
    
    fs.addFunction("quux", {"a", "b", "c"});
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
