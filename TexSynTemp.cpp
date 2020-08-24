//
//  TexSynTemp.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/14/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "TexSynTemp.h"


// Simple function set that mimics a portion Texsyn.
FunctionSet tiny_texsyn_object =
{
    {
        {"Float_01", [](){ return frandom01() < 0.5 ? "0" : "1"; }},
        {"Texture"},
        {"Vec2"}
    },
    {
        {"Vec2", "Vec2",
            {"Float_01", "Float_01"}},
        {"Uniform", "Texture",
            {"Float_01", "Float_01", "Float_01"}},
        {"Spot", "Texture",
            {"Vec2", "Float_01", "Texture", "Float_01", "Texture"}},
        {"Add", "Texture", {"Texture", "Texture"}},
        {"Blur", "Texture", {"Float_01", "Texture"}},
        {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}}
    }
};

// Function set that covers the entire Texsyn API.
FunctionSet full_texsyn_object =
{
    {
        {"Texture"},
        {"Vec2"},
        // TODO -- note these used to reference eg "fs.rs().frandom01()"
        //         temporarily they are calling the global (non-RandomSequence)
        //         version, so repeatability is missing.
        {"Float_01", [](){ return std::to_string(frandom01()); }},
        {"Float_02", [](){ return std::to_string(frandom2(0, 2)); }},
        {"Float_0_10", [](){ return std::to_string(frandom2(0, 10)); }},
        {"Float_m5p5", [](){ return std::to_string(frandom2(-5, 5)); }}
    },
    {
        // Functions
        {"Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"}},
        {"Uniform", "Texture",  {"Float_01", "Float_01", "Float_01"}},
        {"Spot", "Texture",
            {"Vec2", "Float_02", "Texture", "Float_02", "Texture"}},
        {"Gradation", "Texture", {"Vec2", "Texture", "Vec2", "Texture"}},
        {"Grating", "Texture",
            {"Vec2", "Texture", "Vec2", "Texture",
                "Float_01", "Float_01"}},
        {"SoftMatte", "Texture", {"Texture", "Texture", "Texture"}},
        {"Add", "Texture", {"Texture", "Texture"}},
        {"Subtract", "Texture", {"Texture", "Texture"}},
        {"Multiply", "Texture", {"Texture", "Texture"}},
        {"Max", "Texture", {"Texture", "Texture"}},
        {"Min", "Texture", {"Texture", "Texture"}},
        {"AbsDiff", "Texture", {"Texture", "Texture"}},
        //{"NotEqual", "Texture", {"Texture", "Texture"}},
        //{"Noise", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
        //{"Brownian", "Texture",
        //               {"Vec2", "Vec2", "Texture", "Texture"}},
        //{"Turbulence", "Texture",
        //               {"Vec2", "Vec2", "Texture", "Texture"}},
        //{"Furbulence", "Texture",
        //               {"Vec2", "Vec2", "Texture", "Texture"}},
        //{"Wrapulence", "Texture",
        //               {"Vec2", "Vec2", "Texture", "Texture"}},
        {"MultiNoise", "Texture",
            {"Vec2", "Vec2", "Texture", "Texture", "Float_01"}},
        {"ColorNoise", "Texture", {"Vec2", "Vec2", "Float_01"}},
        {"BrightnessToHue", "Texture", {"Float_01", "Texture"}},
        {"Wrap", "Texture", {"Float_m5p5", "Vec2", "Vec2", "Texture"}},
        {"StretchSpot", "Texture",
            {"Float_02", "Float_02", "Vec2", "Texture"}},
        {"Stretch", "Texture", {"Vec2", "Vec2", "Texture"}},
        {"SliceGrating", "Texture", {"Vec2", "Vec2", "Texture"}},
        {"SliceToRadial", "Texture", {"Vec2", "Vec2", "Texture"}},
        {"SliceShear", "Texture",
            {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"}},
        {"Colorize", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
        {"MobiusTransform", "Texture",
            {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
        //{"Scale", "Texture", {"Float_m5p5", "Texture"}},
        //{"Rotate", "Texture", {"Float_m5p5", "Texture"}},
        //{"Translate", "Texture", {"Vec2", "Texture"}},
        {"Blur", "Texture", {"Float_01", "Texture"}},
        {"SoftThreshold", "Texture", {"Float_01", "Float_01", "Texture"}},
        {"EdgeDetect", "Texture", {"Float_01", "Texture"}},
        {"EdgeEnhance", "Texture", {"Float_01", "Float_02", "Texture"}},
        {"AdjustHue", "Texture", {"Float_01", "Texture"}},
        {"AdjustSaturation", "Texture", {"Float_01", "Texture"}},
        {"AdjustBrightness", "Texture", {"Float_01", "Texture"}},
        {"Twist", "Texture", {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"}},
        {"BrightnessWrap", "Texture", {"Float_01", "Float_01", "Texture"}},
        {"Mirror", "Texture", {"Vec2", "Vec2", "Texture"}},
        {"Ring", "Texture", {"Float_0_10", "Vec2", "Vec2", "Texture"}},
        {"Row", "Texture", {"Vec2", "Vec2", "Texture"}},
        //{"Shader", "Texture", {}},
        // TODO reconsider the ephemeral float ranges.
        {"LotsOfSpots", "Texture",
            {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                "Texture", "Texture"}},
        // TODO reconsider the ephemeral float ranges.
        {"ColoredSpots", "Texture",
            {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                "Texture", "Texture"}},
        // TODO reconsider the ephemeral float ranges.
        {"LotsOfButtons", "Texture",
            {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                "Vec2", "Texture", "Float_01", "Texture"}},
        {"Gamma", "Texture", {"Float_0_10", "Texture"}},
        {"RgbBox", "Texture",
            {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                "Float_01", "Texture"}},
        {"CotsMap", "Texture", {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
        // TODO reconsider the ephemeral float ranges.
        {"Hyperbolic", "Texture",
            {"Vec2", "Float_0_10", "Float_0_10", "Float_0_10",
                "Texture", "Texture"}},
        {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}},
        {"HueOnly", "Texture", {"Float_01", "Float_01", "Texture"}}
    }
};

// Simple function set that mimics a portion Texsyn.
FunctionSet& tiny_texsyn() { return tiny_texsyn_object; }

// Function set that covers the entire Texsyn API.
FunctionSet& full_texsyn() { return full_texsyn_object; }
