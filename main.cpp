//
//  main.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/5/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"
#include "TexSynTemp.h"

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
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // Prototyping “make random program” for FunctionSet.
//    std::cout << "August 11, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200811_";
//    FunctionSet fs;
//    auto ephemeral_float_m5_5 = [&]()
//    {
//        std::cout << fs.rs().frandom2(-5, 5);
//    };
//    // Simple function set that mimics Texsyn
//    fs.addType("Float");
//    fs.addType("Vec2");
//    fs.addType("Texture");
//    // with three parameters
//    fs.addFunction("Vec2", "Vec2", {"Float", "Float"});
//    // with one FD parameter
//    fs.addFunction({"Float_m5_5", "Float", {}, ephemeral_float_m5_5});
//    fs.addFunction({"Float_01", "Float_01", {},
//                    [&](){ std::cout << fs.rs().frandom01(); }});
//    fs.addFunction("Uniform", "Texture", {"Float_01", "Float_01", "Float_01"});
//    fs.addFunction({"Multiply", "Texture", {"Texture", "Texture"}});
//    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
//    fs.addFunction("Scale", "Texture", {"Float", "Texture"});
//    fs.printSet();
//    for (int i = 0; i < 20; i++)
//        debugPrint(fs.randomFunctionReturningType("Texture"));
//
//    for (int i = 0; i < 10; i++)
//    {
//        int actual_size = 0;
//        fs.makeRandomProgram(20, "Texture", actual_size);
//        std::cout << std::endl << "  size=" << actual_size << std::endl;
//    }
//
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
    
//    // Debugging max_size for FunctionSet::makeRandomProgram().
//    std::cout << "August 17, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200817_";
//
//    FunctionSet tiny_texsyn_fs;
//    make_tiny_texsyn_fs(tiny_texsyn_fs);
//    std::cout << std::endl;
//    FunctionSet full_texsyn_fs;
//    make_full_texsyn_fs(full_texsyn_fs);
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("LotsOfSpots"))
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("ColorNoise"))
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("Grating"))
//    for (int i = 0; i < 20; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        full_texsyn_fs.dp_depth = 0;
//        full_texsyn_fs.makeRandomProgram(50, "Texture", actual_size, source_code);
//        std::cout << std::endl << std::endl << source_code << std::endl;
//        std::cout << "size=" << actual_size << std::endl;
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Generalizing FunctionSet::minSizeToTerminateType().
//    std::cout << "August 18, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200818_";
//
//    /*
//    FunctionSet tiny_texsyn_fs;
//    make_tiny_texsyn_fs(tiny_texsyn_fs);
//    std::cout << std::endl;
//    FunctionSet full_texsyn_fs;
//    make_full_texsyn_fs(full_texsyn_fs);
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("LotsOfSpots"))
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("ColorNoise"))
//    debugPrint(full_texsyn_fs.minSizeToTerminateFunction("Grating"))
//
//    for (int i = 0; i < 20; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        full_texsyn_fs.dp_depth = 0;
//        full_texsyn_fs.makeRandomProgram(50, "Texture", actual_size, source_code);
//        std::cout << std::endl << std::endl << source_code << std::endl;
//        std::cout << "size=" << actual_size << std::endl;
//    }
//
//    std::set<FunctionType> set_of_function_types;
//    full_texsyn_fs.fillInSetOfTypes(set_of_function_types);
//    std::cout << "set_of_function_types={ ";
//    for (auto& type : set_of_function_types) std::cout << type << " ";
//    std::cout << "}" << std::endl << std::endl;
//
//    for (auto& type : set_of_function_types)
//    {
//        std::vector<std::string> functions;
//        full_texsyn_fs.findAllFunctionReturningType(type, functions);
//        std::cout << type << " " << functions.size() << std::endl;
//    }
//    */
//
//    std::cout << std::endl << "FunctionSet foo:" << std::endl;
//    // FunctionSet& foo = TexSynFS::tiny();
//    FunctionSet& foo = TexSynFS::full();
//    foo.printSet();
//
//    for (int i = 0; i < 20; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        foo.makeRandomProgram(50, "Texture", actual_size, source_code);
//        std::cout << std::endl << std::endl << source_code << std::endl;
//        std::cout << "size=" << actual_size << std::endl;
//    }
//
//    // TODO (recorded Aug 22) should end with:
//    // Spot(Vec2(1, 1), 0, Uniform(1, 0, 0), 0, Add(Affine(Vec2(1, 0),
//    //      Vec2(1, 0), Add(Uniform(1, 1, 1), Blur(1, Uniform(0, 0, 0)))),
//    //      Affine(Vec2(0, 0), Vec2(0, 1),
//    //      Spot(Vec2(1, 1), 1, Uniform(1, 0, 1), 1, Uniform(1, 0, 1)))))
//    // size=50

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Prototyping GpTree
//    std::cout << "August 28, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200828_";
//
//    debugPrint(__cplusplus);
//    FunctionSet& fs = TexSynFS::full();
//
//    for (int i = 0; i < 1; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//
//        GpTree gp_tree;
//        fs.makeRandomProgram(50, "Texture", actual_size, source_code, gp_tree);
//
//        std::cout << std::endl << std::endl << source_code << std::endl;
//        std::cout << "size=" << actual_size << std::endl;
//        debugPrint(gp_tree.size());
//        debugPrint(gp_tree.to_string());
//    }
//
//    // Hmm this isn't matching end of day Friday, but I did make changes to
//    // UnitTests, so sequence might be off from when I record this earlier?
//    /*
//     Ring(4.638507, Vec2(4.394670, 1.221711), Vec2(3.301481, -1.996337), Blur(0.255947, Colorize(Vec2(2.071771, 0.246918), Vec2(-0.048487, -4.911816), Hyperbolic(Vec2(-2.895018, 3.438483), 5.581370, 6.095272, 3.242264, Uniform(0.273231, 0.186754, 0.774368), Uniform(0.808660, 0.145623, 0.479601)), Min(AdjustHue(0.648172, AdjustHue(0.823067, Uniform(0.292994, 0.357870, 0.713751))), Multiply(Uniform(0.009821, 0.054191, 0.788432), Uniform(0.175616, 0.581249, 0.055102))))))
//     size=50
//     */
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Prototyping GpTree::to_string(). Testing case where a type is returned
//    // by some functions and also has an ephemeral generator. (TexSyn has none)
//    std::cout << "August 29, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200829_";
//
//    debugPrint(__cplusplus);
//
//    // std::string root_type = "Texture";
//    // FunctionSet& fs = TexSynFS::full();
//
//    std::string root_type = "Int";
//    FunctionSet fs =
//    {
//        {
//            {"Int", [](){ return std::to_string(rand() % 10); }}
//        },
//        {
//            {"Ant", "Int", {"Int", "Int"}},
//            {"Bat", "Int", {"Int", "Int"}},
//            {"Cat", "Int", {"Int"}}
//        }
//    };
//
//    for (int i = 0; i < 10; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        GpTree gp_tree;
//        fs.makeRandomProgram(50, root_type, actual_size, source_code, gp_tree);
//        std::cout << std::endl << std::endl << source_code << std::endl;
//        std::cout << "size=" << actual_size << std::endl;
//        debugPrint(gp_tree.size());
//        debugPrint(gp_tree.to_string());
//    }
//
//    /*
//     Cat(Cat(Cat(Ant(Cat(Cat(Bat(Bat(Ant(4, Cat(7)), Ant(Cat(4), Cat(4))), Cat(Cat(Bat(Bat(4, 8), Cat(Cat(Cat(8))))))))), Cat(Bat(Ant(Ant(1, Cat(7)), Ant(Cat(3), Cat(2))), Ant(Bat(Cat(0), Cat(9)), Cat(Bat(7, Cat(4))))))))))
//     size=50
//     */
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // Prototyping templated version of GpType
    std::cout << "August 30, 2020" << std::endl;
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200830_";
    
    debugPrint(__cplusplus);

    TestGpType<float> foo("foo", [](){ return frandom01(); });
    TestGpType<int> bar("bar");
//    debugPrint(foo.hasEphemeralGenerator());
//    debugPrint(bar.hasEphemeralGenerator());

//    std::vector<GpType*> q;
//    q.push_back(&foo);
//    q.push_back(&bar);
    
//    std::vector<TestGpType<>*> v;
    
    std::vector<GpType*> gp_types = {&foo, &bar};
//    for (auto& t : vector_of_GpTypes) debugPrint(t->hasEphemeralGenerator());

    
//    dynamic_cast<TestGpType<float>*>(gp_types.at(0))->hasEphemeralGenerator();
    debugPrint(foo.hasEphemeralGenerator());
    debugPrint(bar.hasEphemeralGenerator());

    debugPrint(foo.generateEphemeralConstant());
    debugPrint(foo.generateEphemeralConstant());
    debugPrint(foo.generateEphemeralConstant());

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
