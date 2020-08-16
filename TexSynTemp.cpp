//
//  TexSynTemp.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/14/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "TexSynTemp.h"

// Simple function set that mimics Texsyn
void make_tiny_texsyn_fs(FunctionSet& fs)
{
    // Types (TODO note that these are currently ignored).
    fs.addType("Float");
    fs.addType("Vec2");
    fs.addType("Texture");
    // Ephemeral constants (floats on different random ranges)
    fs.addFunction("Float_01", "Float_01", {},
                   [&](){ return std::to_string(fs.rs().frandom01()); });
    fs.addFunction("Float_m5p5", "Float_m5p5", {},
                   [&](){ return std::to_string(fs.rs().frandom2(-5, 5)) ;});
    // Functions
    fs.addFunction("Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"});
    fs.addFunction("Uniform", "Texture", {"Float_01", "Float_01", "Float_01"});
    fs.addFunction("Multiply", "Texture", {"Texture", "Texture"});
    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("Scale", "Texture", {"Float_m5p5", "Texture"});
    fs.printSet();
}

void make_full_texsyn_fs(FunctionSet& fs)
{
    // Types (TODO note that these are currently ignored).
    fs.addType("Float");
    fs.addType("Vec2");
    fs.addType("Texture");
    // Ephemeral constants (floats on different random ranges)
    fs.addFunction("Float_01", "Float_01", {},
                   [&](){ return std::to_string(fs.rs().frandom01()); });
    fs.addFunction("Float_02", "Float_02", {},
                   [&](){ return std::to_string(fs.rs().frandom2(0, 2)); });
    fs.addFunction("Float_0_10", "Float_0_10", {},
                   [&](){ return std::to_string(fs.rs().frandom2(0, 10)); });
    fs.addFunction("Float_m5p5", "Float_m5p5", {},
                   [&](){ return std::to_string(fs.rs().frandom2(-5, 5)); });
    // Functions
    fs.addFunction("Vec2", "Vec2",
                   {"Float_m5p5", "Float_m5p5"});
    fs.addFunction("Uniform", "Texture",
                   {"Float_01", "Float_01", "Float_01"});
    fs.addFunction("Spot", "Texture",
                   {"Vec2", "Float_02", "Texture", "Float_02", "Texture"});
    fs.addFunction("Gradation", "Texture",
                   {"Vec2", "Texture", "Vec2", "Texture"});
    fs.addFunction("Grating", "Texture",
                   {"Vec2", "Texture", "Vec2", "Texture",
                    "Float_01", "Float_01"});
    fs.addFunction("SoftMatte", "Texture", {"Texture", "Texture", "Texture"});
    fs.addFunction("Add", "Texture", {"Texture", "Texture"});
    fs.addFunction("Subtract", "Texture", {"Texture", "Texture"});
    fs.addFunction("Multiply", "Texture", {"Texture", "Texture"});
    fs.addFunction("Max", "Texture", {"Texture", "Texture"});
    fs.addFunction("Min", "Texture", {"Texture", "Texture"});
    fs.addFunction("AbsDiff", "Texture", {"Texture", "Texture"});
    //fs.addFunction("NotEqual", "Texture", {"Texture", "Texture"});
    //fs.addFunction("Noise", "Texture", {"Vec2", "Vec2", "Texture", "Texture"});
    //fs.addFunction("Brownian", "Texture",
    //               {"Vec2", "Vec2", "Texture", "Texture"});
    //fs.addFunction("Turbulence", "Texture",
    //               {"Vec2", "Vec2", "Texture", "Texture"});
    //fs.addFunction("Furbulence", "Texture",
    //               {"Vec2", "Vec2", "Texture", "Texture"});
    //fs.addFunction("Wrapulence", "Texture",
    //               {"Vec2", "Vec2", "Texture", "Texture"});
    fs.addFunction("MultiNoise", "Texture",
                   {"Vec2", "Vec2", "Texture", "Texture", "Float_01"});
    fs.addFunction("ColorNoise", "Texture", {"Vec2", "Vec2", "Float_01"});
    fs.addFunction("BrightnessToHue", "Texture", {"Float_01", "Texture"});
    fs.addFunction("Wrap", "Texture", {"Float_m5p5", "Vec2", "Vec2", "Texture"});
    fs.addFunction("StretchSpot", "Texture",
                   {"Float_02", "Float_02", "Vec2", "Texture"});
    fs.addFunction("Stretch", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("SliceGrating", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("SliceToRadial", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("SliceShear", "Texture",
                   {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"});
    fs.addFunction("Colorize", "Texture",
                   {"Vec2", "Vec2", "Texture", "Texture"});
    fs.addFunction("MobiusTransform", "Texture",
                   {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"});
    //fs.addFunction("Scale", "Texture", {"Float_m5p5", "Texture"});
    //fs.addFunction("Rotate", "Texture", {"Float_m5p5", "Texture"});
    //fs.addFunction("Translate", "Texture", {"Vec2", "Texture"});
    fs.addFunction("Blur", "Texture", {"Float_01", "Texture"});
    fs.addFunction("SoftThreshold", "Texture",
                   {"Float_01", "Float_01", "Texture"});
    fs.addFunction("EdgeDetect", "Texture", {"Float_01", "Texture"});
    fs.addFunction("EdgeEnhance", "Texture",
                   {"Float_01", "Float_02", "Texture"});
    fs.addFunction("AdjustHue", "Texture", {"Float_01", "Texture"});
    fs.addFunction("AdjustSaturation", "Texture", {"Float_01", "Texture"});
    fs.addFunction("AdjustBrightness", "Texture", {"Float_01", "Texture"});
    fs.addFunction("Twist", "Texture",
                   {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"});
    fs.addFunction("BrightnessWrap", "Texture",
                   {"Float_01", "Float_01", "Texture"});
    fs.addFunction("Mirror", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("Ring", "Texture",
                   {"Float_0_10", "Vec2", "Vec2", "Texture"});
    fs.addFunction("Row", "Texture", {"Vec2", "Vec2", "Texture"});
    //fs.addFunction("Shader", "Texture", {});
    // TODO reconsider the ephemeral float ranges.
    fs.addFunction("LotsOfSpots", "Texture",
                   {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Texture", "Texture"});
    // TODO reconsider the ephemeral float ranges.
    fs.addFunction("ColoredSpots", "Texture",
                   {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Texture", "Texture"});
    // TODO reconsider the ephemeral float ranges.
    fs.addFunction("LotsOfButtons", "Texture",
                   {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Vec2", "Texture", "Float_01", "Texture"});
    fs.addFunction("Gamma", "Texture", {"Float_0_10", "Texture"});
    fs.addFunction("RgbBox", "Texture",
                   {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Float_01", "Texture"});
    fs.addFunction("CotsMap", "Texture",
                   {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"});
    // TODO reconsider the ephemeral float ranges.
    fs.addFunction("Hyperbolic", "Texture",
                   {"Vec2", "Float_0_10", "Float_0_10", "Float_0_10",
                    "Texture", "Texture"});
    fs.addFunction("Affine", "Texture", {"Vec2", "Vec2", "Texture"});
    fs.addFunction("HueOnly", "Texture", {"Float_01", "Float_01", "Texture"});

    fs.printSet();
}
