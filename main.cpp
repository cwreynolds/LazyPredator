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
    
    auto ephemeral_float_m5_5 = [&]()
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
//    fs.addFunction({"EphemeralFloat", "Float", {}, ephemeral_float_m5_5});
    fs.addFunction({"Float_m5_5", "Float", {}, ephemeral_float_m5_5});
    
    fs.addFunction({"Float_01", "Float_01", {},
                    [&](){ std::cout << fs.rs().frandom01(); }});

//    fs.addFunction("Uniform", "Texture", {});
//    fs.addFunction("Uniform", "Texture", {"Float", "Float", "Float"});
    fs.addFunction("Uniform", "Texture", {"Float_01", "Float_01", "Float_01"});

    fs.addFunction({"Multiply", "Texture", {"Texture", "Texture"}});
    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("Scale", "Texture", {"Float", "Texture"});
    fs.printSet();
    for (int i = 0; i < 20; i++)
        debugPrint(fs.randomFunctionReturningType("Texture"));
    
    for (int i = 0; i < 10; i++)
    {
        int actual_size = 0;
        fs.makeRandomProgram(20, "Texture", actual_size);
        std::cout << std::endl << "  size=" << actual_size << std::endl;
    }
    
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
    
    // 11am Aug 13
//    Affine(Vec2(1.48294, -0.682632), Vec2(2.19841, 0.204911),
//           Affine(Vec2(1.62522, -4.33834), Vec2(0.836596, 3.15212),
//                  Affine(Vec2(4.27503, 4.35545), Vec2(0.060585, -1.09821),
//                         Scale(-0.421922,
//                               Affine(Vec2(4.79765, 2.64766),
//                                      Vec2(-3.55406, -4.87557),
//                                      Uniform(-4.37138, 4.62954, 1.69687))))))
    
    // 3:30 Aug 13
//    Scale(-3.49103,
//          Multiply(Scale(-2.20811,
//                         Affine(Vec2(0.0481653, 4.41047),
//                                Vec2(3.97037, 4.67153),
//                                Uniform(0.329659, 4.31111, 4.7829))),
//                   Uniform(3.53818, -4.40229, -3.59649)))
    
    // 4pm aug 13
    
//Affine(Vec2(1.62775, 0.925812),
//       Vec2(0.892051, -2.5576),
//       Scale(-3.9347,
//             Affine(Vec2(1.14234, 0.321014),
//                    Vec2(3.70146, 3.92498),
//                    Uniform(0.842375, 0.46032, 0.180332))))
    
//    Affine(Vec2(1.62775, 0.925812),
//           Vec2(0.892051, -2.5576),
//           Scale(-3.9347,
//                 Affine(Vec2(1.14234, 0.321014),
//                        Vec2(3.70146, 3.92498),
//                        Uniform(0.842375, 0.46032, 0.180332))))
//
//    Scale(-1.03888,
//          Scale(-3.94829,
//                Multiply(Uniform(0.248677, 0.0252578, 0.525256),
//                         Affine(Vec2(1.98506, 1.5941),
//                                Vec2(-0.294979, -1.85345),
//                                Uniform(0.377369, 0.79659, 0.0605764)))))
//
//    Scale(-1.89065,
//          Affine(Vec2(2.71928, 1.51213),
//                 Vec2(-2.15447, 3.58087),
//                 Multiply(Scale(-0.46497,
//                                Uniform(0.270371, 0.544808, 0.653164)),
//                          Uniform(0.582032, 0.0811457, 0.593893))))
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
