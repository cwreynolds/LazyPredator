//
//  TestFS.h
//  LazyPredator
//
//  Created by Craig Reynolds on 9/10/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

//  Sample FunctionSets for testing and as examples.

#pragma once
#include "FunctionSet.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO do the class instance values returned from GpFunctions want to be:
//      return std::any(Uniform(...));
//      return std::any(new Uniform(...));
//      return std::any(*(new Uniform(...)));
//
//      I think std::any(Vec2()) is fine, but for classes derived from Texture?
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class TestFS
{
public:
    // Simple test with only Float and Int types which can be evaluated.
    static const FunctionSet& testTreeEval() { return test_tree_eval; }
    
    // Subset of TexSyn API
    static const FunctionSet& tinyTexSyn() { return tiny_texsyn; }
    // Covers "most" of TexSyn API.
    static const FunctionSet& fullTexSyn() { return full_texsyn; }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    class ClassC
    {
    public:
        ClassC(int i, int j) : i_(i), j_(j) {}
        std::string originStory() const
        {
            return ("[ClassC made from " + std::to_string(i_) +
                    " and " + std::to_string(j_) + "]");
        }
    private:
        int i_;
        int j_;
    };
    class ClassB
    {
    public:
        ClassB(float f) : f_(f) {}
        std::string originStory() const
        {
            return "[ClassB made from " + std::to_string(f_) + "]";
        }
    private:
        float f_;
    };
    class ClassA
    {
    public:
        ClassA(const ClassB& b, ClassC c) : b_(b), c_(c) {}
        std::string originStory() const
        {
            return ("[ClassC made from " + b_.originStory() +
                    " and " + c_.originStory() + "]");
        }
    private:
        const ClassB& b_;
        const ClassC c_;
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Moved "Float" to top in case we want to use that convention.
    // std::string root_type = "Float";
    static inline const FunctionSet test_tree_eval =
    {
        {
            {
                "Float",
                [](){ return std::any(frandom01()); },
                any_to_string<float>
            },
            {
                "Int",
                [](){ return std::any(int(rand() % 10)); },
                any_to_string<int>
            }
        },
        {
            {
                "AddInt", "Int", {"Int", "Int"}, [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "AddFloat", "Float", {"Float", "Float"}, [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<float>(0) +
                                    t.evalSubtree<float>(1));
                }
            },
            {
                "Floor", "Int", {"Float"}, [](const GpTree& t)
                {
                    return std::any(int(std::floor(t.evalSubtree<float>(0))));
                }
            },
            {
                "Sqrt", "Float", {"Int"}, [](const GpTree& t)
                {
                    return std::any(float(std::sqrt(t.evalSubtree<int>(0))));
                }
            },
            {
                "Mult", "Float", {"Float", "Int"}, [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<float>(0) *
                                    t.evalSubtree<int>(1));
                }
            }
        }
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    
    static inline const FunctionSet tiny_texsyn =
    {
        {
            {"Texture"},
            {"Vec2"},
            {
                "Float_01",
                [](){ return std::any(int(frandom01() < 0.5 ? 0 : 1)); },
                any_to_string<int>,
            }
//            {"Texture"},
//            {"Vec2"}
        },
        {
            {
                "Vec2", "Vec2", {"Float_01", "Float_01"}, [](const GpTree& t)
                {
                    return std::any(Vec2(t.evalSubtree<float>(0),
                                         t.evalSubtree<float>(1)));
                }
            },
            
//            {
//                "Uniform", "Texture", {"Float_01", "Float_01", "Float_01"},
//                [](const GpTree& t)
//                {
//                    return std::any(*(new Uniform(t.evalSubtree<float>(0),
//                                                  t.evalSubtree<float>(1),
//                                                  t.evalSubtree<float>(2))));
//                }
//            },
            
//            {"Uniform", "Texture", {"Float_01", "Float_01", "Float_01"}},
//            {"Spot", "Texture",
//                {"Vec2", "Float_01", "Texture", "Float_01", "Texture"}},
//            {"Add", "Texture", {"Texture", "Texture"}},
//            {"Blur", "Texture", {"Float_01", "Texture"}},
//            {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}}
        }
    };

    static inline FunctionSet full_texsyn;

//    static FunctionSet& full()
//    {
//        static FunctionSet full =
//        {
//            {
//                {"Texture"},
//                {"Vec2"},
//                // TODO -- note these used to reference eg "fs.rs().frandom01()"
//                //         temporarily they are calling the global (non
//                //         RandomSequence) version, so repeatability is missing.
//                {"Float_01", [](){ return std::to_string(frandom01()); }},
//                {"Float_02", [](){ return std::to_string(frandom2(0, 2)); }},
//                {"Float_0_10", [](){ return std::to_string(frandom2(0, 10)); }},
//                {"Float_m5p5", [](){ return std::to_string(frandom2(-5, 5)); }}
//            },
//            {
//                // Functions
//                {"Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"}},
//                {"Uniform", "Texture",  {"Float_01", "Float_01", "Float_01"}},
//                {"Spot", "Texture",
//                    {"Vec2", "Float_02", "Texture", "Float_02", "Texture"}},
//                {"Gradation", "Texture",
//                    {"Vec2", "Texture", "Vec2", "Texture"}},
//                {"Grating", "Texture",
//                    {"Vec2", "Texture", "Vec2", "Texture",
//                        "Float_01", "Float_01"}},
//                {"SoftMatte", "Texture", {"Texture", "Texture", "Texture"}},
//                {"Add", "Texture", {"Texture", "Texture"}},
//                {"Subtract", "Texture", {"Texture", "Texture"}},
//                {"Multiply", "Texture", {"Texture", "Texture"}},
//                {"Max", "Texture", {"Texture", "Texture"}},
//                {"Min", "Texture", {"Texture", "Texture"}},
//                {"AbsDiff", "Texture", {"Texture", "Texture"}},
//                //{"NotEqual", "Texture", {"Texture", "Texture"}},
//                //{"Noise", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Brownian", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Turbulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Furbulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Wrapulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                {"MultiNoise", "Texture",
//                    {"Vec2", "Vec2", "Texture", "Texture", "Float_01"}},
//                {"ColorNoise", "Texture", {"Vec2", "Vec2", "Float_01"}},
//                {"BrightnessToHue", "Texture", {"Float_01", "Texture"}},
//                {"Wrap", "Texture", {"Float_m5p5", "Vec2", "Vec2", "Texture"}},
//                {"StretchSpot", "Texture",
//                    {"Float_02", "Float_02", "Vec2", "Texture"}},
//                {"Stretch", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceGrating", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceToRadial", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceShear", "Texture",
//                    {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"}},
//                {"Colorize", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
//                {"MobiusTransform", "Texture",
//                    {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
//                //{"Scale", "Texture", {"Float_m5p5", "Texture"}},
//                //{"Rotate", "Texture", {"Float_m5p5", "Texture"}},
//                //{"Translate", "Texture", {"Vec2", "Texture"}},
//                {"Blur", "Texture", {"Float_01", "Texture"}},
//                {"SoftThreshold", "Texture",
//                    {"Float_01", "Float_01", "Texture"}},
//                {"EdgeDetect", "Texture", {"Float_01", "Texture"}},
//                {"EdgeEnhance", "Texture", {"Float_01", "Float_02", "Texture"}},
//                {"AdjustHue", "Texture", {"Float_01", "Texture"}},
//                {"AdjustSaturation", "Texture", {"Float_01", "Texture"}},
//                {"AdjustBrightness", "Texture", {"Float_01", "Texture"}},
//                {"Twist", "Texture",
//                    {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"}},
//                {"BrightnessWrap", "Texture",
//                    {"Float_01", "Float_01", "Texture"}},
//                {"Mirror", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"Ring", "Texture", {"Float_0_10", "Vec2", "Vec2", "Texture"}},
//                {"Row", "Texture", {"Vec2", "Vec2", "Texture"}},
//                //{"Shader", "Texture", {}},
//                // TODO reconsider the ephemeral float ranges.
//                {"LotsOfSpots", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Texture", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"ColoredSpots", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Texture", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"LotsOfButtons", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Vec2", "Texture", "Float_01", "Texture"}},
//                {"Gamma", "Texture", {"Float_0_10", "Texture"}},
//                {"RgbBox", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Float_01", "Texture"}},
//                {"CotsMap", "Texture", {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"Hyperbolic", "Texture",
//                    {"Vec2", "Float_0_10", "Float_0_10", "Float_0_10",
//                        "Texture", "Texture"}},
//                {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"HueOnly", "Texture", {"Float_01", "Float_01", "Texture"}}
//            }
//        };
//        return full;
//    }
};
