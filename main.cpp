//
//  main.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/5/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "LazyPredator.h"
#include "TestFS.h"

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
    
//    // Prototyping templated version of GpType
//    std::cout << "August 30, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200830_";
//
//    debugPrint(__cplusplus);
//
//    TestGpType<float> foo("foo", [](){ return frandom01(); });
//    TestGpType<int> bar("bar");
//    //    debugPrint(foo.hasEphemeralGenerator());
//    //    debugPrint(bar.hasEphemeralGenerator());
//
//    //    std::vector<GpType*> q;
//    //    q.push_back(&foo);
//    //    q.push_back(&bar);
//
//    //    std::vector<TestGpType<>*> v;
//
//    std::vector<GpType*> gp_types = {&foo, &bar};
//    //    for (auto& t : vector_of_GpTypes) debugPrint(t->hasEphemeralGenerator());
//
//
//    //dynamic_cast<TestGpType<float>*>(gp_types.at(0))->hasEphemeralGenerator();
//    debugPrint(foo.hasEphemeralGenerator());
//    debugPrint(bar.hasEphemeralGenerator());
//
//    debugPrint(foo.generateEphemeralConstant());
//    debugPrint(foo.generateEphemeralConstant());
//    debugPrint(foo.generateEphemeralConstant());

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Prototyping templated version of GpType
//    std::cout << "August 30, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200830_";
//
//    debugPrint(__cplusplus);
//
//    TestGpType<float> foo("foo", [](){ return frandom01(); });
//    TestGpType<int> bar("bar");
//    std::vector<GpType*> gp_types = {&foo, &bar};
//    GpType& foo2 = static_cast<GpType&>(foo);
//    debugPrint(foo2.name());
//
//    static_cast<TestGpType<float>&>(*gp_types.at(0)).hasEphemeralGenerator();
//    TestGpType<float>& types0 = static_cast<TestGpType<float>&>(*gp_types.at(0));
//    debugPrint(types0.name());
//
//    TestTemplateGpType();
//    VeryTemporaryPrototyping();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // New model assuming types and functions are user-defined classes
//    std::cout << "September 4, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200904_";
//
//    //    MyIntType my_int_type("Int");
//    //    debugPrint(*(my_int_type.generateEphemeralConstant().get<int*>()));
//    //    debugPrint(*(my_int_type.generateEphemeralConstant().get<int*>()));
//    //    debugPrint(*(my_int_type.generateEphemeralConstant().get<int*>()));
//
//    MyFloat01Type mft("Float01");
//    //    debugPrint(mft.get(mft.generateEphemeralConstant()));
//
//
//    const float d = 1.2;
//    std::any var = d;
//    debugPrint(std::any_cast<float>(var));
//    const std::string str = "Hello World";
//    var = str;
//    debugPrint(std::any_cast<std::string>(var));
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Experimenting with std::any
//    std::cout << "September 5, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200905_";
//
//    std::string root_type = "Int";
//    FunctionSet fs =
//    {
//        {
////            {"Int", [](){ return std::to_string(rand() % 10); }}
//            {"Int", nullptr, [](){ return std::any(int(rand() % 10)); }}
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
//    std::variant<int, float> v;
//    v = 5;
//    debugPrint(std::get<int>(v));
//    debugPrint(std::get<0>(v));
//    //debugPrint(std::get<1>(v));
//
//    class Foo
//    {
//    public:
//        Foo(){};
//        Foo(int i) : i_(i) {};
//        int getI() const { return i_; }
//    private:
//        int i_ = 0;
//    };
//
//    std::variant<int, float, Foo> u;
//
//    u = Foo(2);
//    debugPrint(std::get<Foo>(u).getI());
//    debugPrint(std::get<2>(u).getI());
//    constexpr int j = 2;
//    debugPrint(std::get<j>(u).getI());
//    //size_t k = 2;
//    //debugPrint(std::get<k>(u).getI());

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Experimenting with eval()
//    std::cout << "September 7, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200907_";
//
//
//    // lets call this FunctionSet test_tree_eval
//    std::string root_type = "Float";
//    FunctionSet fs =
//    {
//        {
//            {
//                "Int",
//                [](){ return std::any(int(rand() % 10)); },
//                any_to_string<int>
//            },
//            {
//                "Float",
//                [](){ return std::any(frandom01()); },
//                any_to_string<float>
//            }
//        },
//        {
//            {
//                "AddInt", "Int", {"Int", "Int"}, [](const GpTree& t)
//                {
//                    return std::any(t.evalSubtree<int>(0) +
//                                    t.evalSubtree<int>(1));
//                }
//            },
//            {
//                "AddFloat", "Float", {"Float", "Float"}, [](const GpTree& t)
//                {
//                    return std::any(t.evalSubtree<float>(0) +
//                                    t.evalSubtree<float>(1));
//                }
//            },
//            {
//                "Floor", "Int", {"Float"}, [](const GpTree& t)
//                {
//                    return std::any(int(std::floor(t.evalSubtree<float>(0))));
//                }
//            },
//            {
//                "Sqrt", "Float", {"Int"}, [](const GpTree& t)
//                {
//                    return std::any(float(std::sqrt(t.evalSubtree<int>(0))));
//                }
//            },
//            {
//                "Mult", "Float", {"Float", "Int"}, [](const GpTree& t)
//                {
//                    return std::any(t.evalSubtree<float>(0) *
//                                    t.evalSubtree<int>(1));
//                }
//            }
//        }
//    };
//
//    std::cout << std::endl;
//    fs.print();
//    std::cout << std::endl;
//
//    for (int i = 0; i < 10; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        GpTree gp_tree;
//        fs.makeRandomProgram(100, root_type, actual_size, source_code, gp_tree);
//        assert(actual_size == gp_tree.size());
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::cout << "eval=" << std::any_cast<float>(gp_tree.eval()) << std::endl;
//    }
//
//    // AAAAHA! first successful eval:
//    //
//    //    AddFloat(AddFloat(0.072686, 0.631635),
//    //             AddFloat(0.884707,
//    //                      AddFloat(0.272710, 0.436411)))
//    //    size=9
//    //    eval=2.29815
//    //
//    // When I checked it with my calculator it was within roundoff!
//
//    // Now with whole test_tree_eval FunctionSet:
//    //
//    //    Sqrt(AddInt(AddInt(6, Floor(0.262453)), AddInt(7, Floor(0.736082))))
//    //    size=10
//    //    eval=3.60555
//    //
//    // and indeed sqrt(6 + 7) = 3.60555
//
//    //AddFloat(Mult(Mult(AddFloat(AddFloat(Sqrt(4), Sqrt(0)), Sqrt(Floor(Mult(0.081061, 5)))),
//    //                   Floor(AddFloat(AddFloat(Sqrt(7), Sqrt(5)),
//    //                                  Mult(Sqrt(0), Floor(0.269215))))),
//    //              Floor(AddFloat(Mult(AddFloat(Sqrt(4), Sqrt(7)), Floor(AddFloat(0.776866, Sqrt(4)))), AddFloat(Sqrt(Floor(AddFloat(0.422460, 0.282156))), Sqrt(Floor(AddFloat(0.193967, 0.011316))))))), AddFloat(Sqrt(Floor(Mult(Mult(Mult(0.191824, Floor(0.983236)), Floor(AddFloat(0.244054, Sqrt(1)))), Floor(Mult(Sqrt(AddInt(8, 5)), AddInt(Floor(0.601010), Floor(0.176880))))))), AddFloat(Sqrt(AddInt(AddInt(Floor(0.828355), Floor(0.157731)), AddInt(Floor(0.987937), Floor(0.257169)))), Sqrt(Floor(AddFloat(Sqrt(Floor(Sqrt(7))), AddFloat(Sqrt(7), Sqrt(3))))))))
//    //size=100
//    //eval=74.2361
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Moving all test FunctionSets to TestFS.
//    std::cout << "September 11, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/202009011_";
//
//    // lets call this FunctionSet test_tree_eval
//    std::string root_type = "Float";
//    // Ideally this should be just a reference to FS in TestFS. Copy for now.
//    FunctionSet fs = TestFS::testTreeEval();
//
//    std::cout << std::endl;
//    fs.print();
//    std::cout << std::endl;
//
//    for (int i = 0; i < 10; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        GpTree gp_tree;
//        fs.makeRandomProgram(100, root_type, actual_size, source_code, gp_tree);
//        assert(actual_size == gp_tree.size());
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::cout << "eval=" << std::any_cast<float>(gp_tree.eval()) << std::endl;
//    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Working on eval() when values involve class instances.
//    std::cout << "September 13, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/202009013_";
//
//    debugPrint(TestFS::ClassC(1, 2).to_string());
//    debugPrint(TestFS::ClassB(0.5).to_string());
//    debugPrint(TestFS::ClassA(TestFS::ClassB(0.5), TestFS::ClassC(1, 2))
//               .to_string());
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Refactor FunctionSet (etc.) to work as const value.
//    std::cout << "September 17, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/202009017_";
//
//    debugPrint(TestFS::ClassC(1, 2).to_string());
//    debugPrint(TestFS::ClassB(0.5).to_string());
//    debugPrint(TestFS::ClassA(TestFS::ClassB(0.5), TestFS::ClassC(1, 2))
//               .to_string());
//
//    std::string root_type = "Float";
//    const FunctionSet& fs = TestFS::treeEval();
//
//    std::cout << std::endl;
//    fs.print();
//    std::cout << std::endl;
//
//    const GpType& type_float = *fs.lookupGpTypeByName("Float");
//    for (auto& gp_function : type_float.functionsReturningThisType())
//    {
//        debugPrint(gp_function->name());
//    }
//    GpFunction* rf = fs.randomFunctionOfTypeInSize(100, type_float);
//    debugPrint(rf);
//    debugPrint(rf->name());
//
//    for (int i = 0; i < 10; i++)
//    {
//        int actual_size = 0;
//        std::string source_code;
//        GpTree gp_tree;
//        fs.makeRandomProgram(100, root_type, actual_size, source_code, gp_tree);
//        assert(actual_size == gp_tree.size());
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::cout << "eval=" << std::any_cast<float>(gp_tree.eval()) << std::endl;
//    }
//
//    // as of Sep 17 am: "...Floor(Mult(0.426497, Floor(0.142021))))))))"
//    // as of Sep 17 am: "...Sqrt(AddInt(Floor(0.904627), Floor(0.081947)))))))
//    //                  eval=43.6148"
//
//    //
//    //     ...AddInt(Floor(0.987937), Floor(0.257169)))),
//    //        Sqrt(Floor(AddFloat(Sqrt(Floor(Sqrt(7))),
//    //        AddFloat(Sqrt(7), Sqrt(3))))))))
//    //     size=100
//    //     eval=74.2361

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Remove FunctionSet::dp, dp_depth, and dp_prefix()
//    std::cout << "September 18, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/202009018_";
//
//    //std::string root_type = "Float";
//    const FunctionSet& fs = TestFS::treeEval();
//    std::cout << std::endl;
//    fs.print();
//    std::cout << std::endl;
//    LPRS().setSeed();
//    for (int i = 0; i < 10; i++)
//    {
//        GpTree gp_tree;
//        // fs.makeRandomTree(100, root_type, gp_tree);
//        fs.makeRandomTree(100, gp_tree);
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::cout << "eval=" << std::any_cast<float>(gp_tree.eval()) << std::endl;
//    }
//
//    // Should end:
//    //    "...AddInt(Floor(0.910863), AddInt(9, 5))))))"
//    //    size=100
//    //    eval=165.879

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Experimenting with crossover
//    std::cout << "September 29, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200929_";
//
//    const FunctionSet& fs = TestFS::crossover();
//    std::cout << std::endl;
//    fs.print();
//    LPRS().setSeed();
//
//    std::string filter_string = "P";
//    filter_string = "Q";
//    FunctionSet::function_filter = [&](std::vector<GpFunction*>& funcs)
//    {
//        std::vector<GpFunction*> temp = funcs;
//        funcs.clear();
//        for (auto& func : temp)
//        {
//            if (func->name().find(filter_string) != std::string::npos)
//                funcs.push_back(func);
//        }
//    };
//
//    //for (int i = 0; i < 10; i++)
//    //{
//    //    GpTree gp_tree;
//    //    fs.makeRandomTree(30, gp_tree);
//    //    std::cout << std::endl << gp_tree.to_string() << std::endl;
//    //    std::cout << "size=" << gp_tree.size() << std::endl;
//    //    std::cout << "eval=" << any_to_string<int>(gp_tree.eval()) << std::endl;
//    //}
//
//    filter_string = "P";
//    GpTree gp_tree_p;
//    fs.makeRandomTree(30, gp_tree_p);
//    debugPrint(gp_tree_p.to_string());
//
//    filter_string = "Q";
//    GpTree gp_tree_q;
//    fs.makeRandomTree(30, gp_tree_q);
//    debugPrint(gp_tree_q.to_string());
//
//    // PPP(PPP(P(7), P(P(1)), PP(4, 3)),
//    //     P(PPP(P(8), P(P(0)), PP(8, 4))),
//    //     PPP(P(P(1)), P(P(6)), P(P(7))))
//    // QQQ(Q(QQQ(Q(7), Q(2), QQ(5, 8))),
//    //     Q(QQQ(Q(5), QQ(5, 3), Q(Q(3)))),
//    //     QQQ(QQ(3, 4), QQ(6, 1), QQ(8, 1)))
//
//
//    debugPrint(gp_tree_p.getSubtree(0).to_string());
//    debugPrint(gp_tree_q.getSubtree(2).to_string());
//
//    gp_tree_p.getSubtree(0) = gp_tree_q.getSubtree(2);
//    debugPrint(gp_tree_p.to_string());
//
//    // PPP(QQQ(QQ(3, 4), QQ(6, 1), QQ(8, 1)),
//    //     P(PPP(P(8), P(P(0)), PP(8, 4))),
//    //     PPP(P(P(1)), P(P(6)), P(P(7))))
//
//    std::cout << std::endl << std::endl;

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Crossover operator.
//    std::cout << "October 1, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201001_";
//
//    const FunctionSet& fs = TestFS::crossover();
//    std::cout << std::endl;
//    fs.print();
//    LPRS().setSeed();
//
//    std::string filter_string = "P";
//    filter_string = "Q";
//    FunctionSet::function_filter = [&](std::vector<GpFunction*>& funcs)
//    {
//        std::vector<GpFunction*> temp = funcs;
//        funcs.clear();
//        for (auto& func : temp)
//        {
//            if (func->name().find(filter_string) != std::string::npos)
//                funcs.push_back(func);
//        }
//    };
//
//    //for (int i = 0; i < 10; i++)
//    //{
//    //    GpTree gp_tree;
//    //    fs.makeRandomTree(30, gp_tree);
//    //    std::cout << std::endl << gp_tree.to_string() << std::endl;
//    //    std::cout << "size=" << gp_tree.size() << std::endl;
//    //    std::cout << "eval=" << any_to_string<int>(gp_tree.eval()) << std::endl;
//    //}
//
//    filter_string = "P";
//    GpTree gp_tree_p;
//    fs.makeRandomTree(30, gp_tree_p);
//    debugPrint(gp_tree_p.to_string());
//
//    filter_string = "Q";
//    GpTree gp_tree_q;
//    fs.makeRandomTree(30, gp_tree_q);
//    debugPrint(gp_tree_q.to_string());
//
//    // PPP(PPP(P(7), P(P(1)), PP(4, 3)),
//    //     P(PPP(P(8), P(P(0)), PP(8, 4))),
//    //     PPP(P(P(1)), P(P(6)), P(P(7))))
//    // QQQ(Q(QQQ(Q(7), Q(2), QQ(5, 8))),
//    //     Q(QQQ(Q(5), QQ(5, 3), Q(Q(3)))),
//    //     QQQ(QQ(3, 4), QQ(6, 1), QQ(8, 1)))
//
//
//    // debugPrint(gp_tree_p.getSubtree(0).to_string());
//    // debugPrint(gp_tree_q.getSubtree(2).to_string());
//
//    // gp_tree_p.getSubtree(0) = gp_tree_q.getSubtree(2);
//    // debugPrint(gp_tree_p.to_string());
//
//    // PPP(QQQ(QQ(3, 4), QQ(6, 1), QQ(8, 1)),
//    //     P(PPP(P(8), P(P(0)), PP(8, 4))),
//    //     PPP(P(P(1)), P(P(6)), P(P(7))))
//
//    std::cout << std::endl;
//
//    GpTree offspring;
//    fs.crossover(gp_tree_p, gp_tree_q, offspring);
//    fs.crossover(gp_tree_p, gp_tree_q, offspring);
//
//    std::cout << std::endl;
//
//    // PPP(PPP(P(7), P(P(1)), PP(4, 3)),
//    //     P(PPP(P(8), P(P(0)), PP(8, 4))),
//    //     QQ(5, 3))

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Guarantee type matching (or detect failure) in crossover operator.
    std::cout << "October 3, 2020" << std::endl;
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201003_";
    
    // const FunctionSet& fs = TestFS::crossover();
    const FunctionSet& fs = TestFS::treeEval();
    std::cout << std::endl;
    fs.print();
    LPRS().setSeed();
    // int max_size = 30;
    int max_size = 15;

    GpTree gp_tree_0;
    fs.makeRandomTree(max_size, gp_tree_0);
    debugPrint(gp_tree_0.to_string());
    
    GpTree gp_tree_1;
    fs.makeRandomTree(max_size, gp_tree_1);
    debugPrint(gp_tree_1.to_string());
        
    std::cout << std::endl;

    GpTree offspring;
    fs.crossover(gp_tree_0, gp_tree_1, offspring);
    fs.crossover(gp_tree_0, gp_tree_1, offspring);

    std::cout << std::endl;
    
//    exchange = 1
//    donor_size = 15
//    donor.to_string() = Sqrt(Floor(Sqrt(Floor(Sqrt(AddInt(Floor(Mult(0.928331, 3)), Floor(Sqrt(Floor(Sqrt(8))))))))))
//    recipient_size = 15
//    recipient.to_string() = Mult(Mult(Sqrt(Floor(0.929361)), AddInt(2, 3)), Floor(AddFloat(Sqrt(6), AddFloat(0.76742, 0.439973))))
//    donor_subtree_index = 3
//    donor_subtree.to_string() = Floor(Sqrt(AddInt(Floor(Mult(0.928331, 3)), Floor(Sqrt(Floor(Sqrt(8)))))))
//    recipient_subtree_index = 5
//    recipient_subtree.to_string() = 6
//    shared types: Float Int
//    offspring.to_string() = Mult(Mult(Sqrt(Floor(0.929361)), AddInt(2, 3)), Floor(AddFloat(Sqrt(Floor(Sqrt(AddInt(Floor(Mult(0.928331, 3)), Floor(Sqrt(Floor(Sqrt(8)))))))), AddFloat(0.76742, 0.439973))))
//    any_to_string<float>(offspring.eval()) = 0
//
//    exchange = 1
//    donor_size = 15
//    donor.to_string() = Sqrt(Floor(Sqrt(Floor(Sqrt(AddInt(Floor(Mult(0.928331, 3)), Floor(Sqrt(Floor(Sqrt(8))))))))))
//    recipient_size = 15
//    recipient.to_string() = Mult(Mult(Sqrt(Floor(0.929361)), AddInt(2, 3)), Floor(AddFloat(Sqrt(6), AddFloat(0.76742, 0.439973))))
//    donor_subtree_index = 7
//    donor_subtree.to_string() = Mult(0.928331, 3)
//    recipient_subtree_index = 8
//    recipient_subtree.to_string() = 0.439973
//    shared types: Float Int
//    offspring.to_string() = Mult(Mult(Sqrt(Floor(0.929361)), AddInt(2, 3)), Floor(AddFloat(Sqrt(6), AddFloat(0.76742, Mult(0.928331, 3)))))
//    any_to_string<float>(offspring.eval()) = 0

    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return EXIT_SUCCESS;
}
