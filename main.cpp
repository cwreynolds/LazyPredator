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
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // Testing FunctionSet.
//    std::cout << "August 9, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200809_";
//    FunctionSet fs;
//
//    // Simple function set that mimics Texsyn
//    fs.addType("Float");
//    fs.addType("Vec2");
//    fs.addType("Texture");
//    fs.addFunction("EphemeralVec2", "Vec2", {});      // with three parameters
//    fs.addFunction({"EphemeralFloat", "Float", {}});  // with one FD parameter
//    fs.addFunction("Uniform", "Texture", {});       // like an ephemeral const
//    fs.addFunction({"Multiply", "Texture", {"Texture", "Texture"}});
//    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
//    fs.printSet();
//    for (int i = 0; i < 20; i++)
//        debugPrint(fs.randomFunctionReturningType("Texture"));
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Prototyping “make random program” for FunctionSet.
    std::cout << "August 11, 2020" << std::endl;
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200811_";
    FunctionSet fs;
    
    // Simple function set that mimics Texsyn
    fs.addType("Float");
    fs.addType("Vec2");
    fs.addType("Texture");
    fs.addFunction("EphemeralVec2", "Vec2", {});      // with three parameters
    fs.addFunction({"EphemeralFloat", "Float", {}});  // with one FD parameter
    fs.addFunction("Uniform", "Texture", {});       // like an ephemeral const
    fs.addFunction({"Multiply", "Texture", {"Texture", "Texture"}});
    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.printSet();
    for (int i = 0; i < 20; i++)
        debugPrint(fs.randomFunctionReturningType("Texture"));
    
    int a = 0;
    int b = 0;
    int c = 0;
    RandomSequence rs;
    for (int i = 0; i < 3000000; i++)
    {
        int r = rs.nextInt() % 3;
        if (r == 0) a++;
        if (r == 1) b++;
        if (r == 2) c++;
    }
    debugPrint(a);
    debugPrint(b);
    debugPrint(c);


    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
