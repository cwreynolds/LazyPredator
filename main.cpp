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
    
    auto ephemeral_float = [&]()
    {
        std::cout << fs.rs().frandom2(-5, 5);
    };

    // Simple function set that mimics Texsyn
    fs.addType("Float");
    fs.addType("Vec2");
    fs.addType("Texture");
    // with three parameters
    fs.addFunction("Vec2", "Vec2", {"Float", "Float"});
    // with one FD parameter
    fs.addFunction({"EphemeralFloat", "Float", {}, ephemeral_float});
    fs.addFunction("Uniform", "Texture", {});       // like an ephemeral const
    fs.addFunction({"Multiply", "Texture", {"Texture", "Texture"}});
    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("Scale", "Texture", {"Float", "Texture"});
    fs.printSet();
    for (int i = 0; i < 20; i++)
        debugPrint(fs.randomFunctionReturningType("Texture"));
    
    fs.makeRandomProgram(10, "Texture");
    std::cout << std::endl;
    fs.makeRandomProgram(10, "Texture");
    std::cout << std::endl;
    
//    Multiply(Affine(EphemeralVec2(),
//                    EphemeralVec2(),
//                    Affine(EphemeralVec2(),
//                           EphemeralVec2(),
//                           Affine(EphemeralVec2(),
//                                  EphemeralVec2(),
//                                  Affine(EphemeralVec2(),
//                                         EphemeralVec2(),
//                                         Multiply(Uniform(),
//                                                  Uniform()))))),
//             Uniform())

//    Scale(4.52324,
//          Scale(-1.24412,
//                Affine(Vec2(4.25972, -1.4378),
//                       Vec2(3.30065, -2.68157),
//                       Multiply(Scale(-0.617392, Uniform()),
//                                Affine(Vec2(-2.40257, 3.26076),
//                                       Vec2(1.59726, 2.31896),
//                                       Scale(-3.55737, Uniform()))))))
    
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
