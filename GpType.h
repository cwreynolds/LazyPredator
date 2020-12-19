//
//  GpType.h
//  LazyPredator
//
//  Created by Craig Reynolds on 12/18/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"

class GpTree;      // Forward reference to class defined later.
class GpFunction;  // Forward reference to class defined later.

// Class to represent types in "strongly typed genetic programming".
class GpType
{
public:
    // Default constructor.
    GpType(){}
    // Constructor for name only.
    GpType(const std::string& name) : name_(name) {}
    // Constructor for name and deleter only.
    GpType(const std::string& name, std::function<void(std::any)> deleter)
      : GpType(name, nullptr, nullptr, nullptr, deleter) {}
    // Constructor for all parameters except deleter.
    GpType(const std::string& name,
           std::function<std::any()> ephemeral_generator,
           std::function<std::string(std::any a)> to_string,
           std::function<std::any(std::any)> jiggle)
      : GpType(name, ephemeral_generator, to_string, jiggle, nullptr) {}
    // Full constructor, specify all parameters.
    GpType(const std::string& name,
           std::function<std::any()> ephemeral_generator,
           std::function<std::string(std::any a)> to_string,
           std::function<std::any(std::any)> jiggle,
           std::function<void(std::any)> deleter)
      : name_(name),
        ephemeral_generator_(ephemeral_generator),
        to_string_(to_string),
        jiggle_(jiggle),
        deleter_(deleter) {}
    // Constructor for ranged numeric types (name, range_min, range_max).
    template <typename T>
    GpType(const std::string& name, T range_min, T range_max)
      : GpType(name, range_min, range_max, defaultJiggleScale()) {}
    // Constructor for ranged numeric types, plus custom jiggle scale.
    template <typename T>
    GpType(const std::string& name, T range_min, T range_max, float jiggle_scale)
      : GpType(name,
               [=](){ return std::any(LPRS().random2(range_min, range_max)); },
               any_to_string<T>,
               [=](std::any x) { return jiggle(std::any_cast<T>(x), range_min,
                                               range_max, jiggle_scale); }){}
    // Accessor for name.
    const std::string& name() const { return name_; }
    // Does this type have an ephemeral generator?
    bool hasEphemeralGenerator() const { return bool(ephemeral_generator_); }
    // Generate an ephemeral constant.
    std::any generateEphemeralConstant() const
    {
        assert(hasEphemeralGenerator());
        return ephemeral_generator_();
    }
    // Does this type have a jiggle function?
    bool hasJiggler() const { return bool(jiggle_); }
    // Generate an ephemeral constant.
    std::any jiggleConstant(std::any current_value) const
    {
        assert(hasJiggler());
        return jiggle_(current_value);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO experimental "deleter" function. EG for Texture in TexSyn.
    
    // Does this type have a deleter function?
    bool hasDeleter() const { return bool(deleter_); }
    // Delete a value (e.g. cached in a GpTree) produced by this GpType.
    // Only needed for pointer/reference to generated instance.
    void deleteValue(std::any value) const
    {
        assert(hasDeleter());
        deleter_(value);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Access collection of (pointers to) GpFunction that return this type.
    const std::vector<GpFunction*>& functionsReturningThisType() const
        { return functions_returning_this_type_; }
    // Add a GpFunction to the collection of those that return this type.
    void addFunctionReturningThisType(GpFunction* gp_function_pointer)
        { functions_returning_this_type_.push_back(gp_function_pointer); }
    // Minimum "size" of tree returning this type from root;
    int minSizeToTerminate() const { return min_size_to_terminate_; }
    void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
    void print() const;
    bool valid() const
    {
        return ((!name().empty()) &&
                // TODO may be wrong given later changes to ephemeral constants:
                (!functionsReturningThisType().empty()) &&
                (minSizeToTerminate() < std::numeric_limits<int>::max()));
    }
    // Uses function (supplied in constructor) to make a string of std::any
    // value via this GpType's concrete c++ type.
    std::string to_string(std::any a) const { return to_string_(a); }
    // Default max jiggle: a scale factor for magnitude of noise added to a
    // ranged numeric GpType by "jiggle mutation." Zero-centered noise added is
    // uniformly distributed on the interval [-m, +m] where m is: ((range_max -
    // range_min) * jiggle_factor). The interval [-m, +m] is then "pre-clipped"
    // to the range of the GpType.
    static float defaultJiggleScale() { return 0.05; }
    // Utility template function for jiggle mutation handlers.
    template <typename T> static T jiggle(T x, T min, T max, float jiggle_scale)
    {
        T max_jiggle = (max - min) * jiggle_scale;
        return LPRS().random2(std::max(min, x - max_jiggle),
                              std::min(max, x + max_jiggle));
    }
private:
    std::string name_;
    // Function to generate an ephemeral constant.
    std::function<std::any()> ephemeral_generator_ = nullptr;
    // Function to generate string representation of a value of this GpType.
    std::function<std::string(std::any a)> to_string_ = nullptr;
    // Function to jiggle/jitter an ephemeral constant.
    std::function<std::any(std::any)> jiggle_ = nullptr;
    // Collection of pointers to GpFunctions which return this type.
    std::vector<GpFunction*> functions_returning_this_type_;
    // Minimum "size" of tree returning this type from root;
    int min_size_to_terminate_ = std::numeric_limits<int>::max();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO experimental "deleter" function. EG for Texture in TexSyn.
    std::function<void(std::any)> deleter_ = nullptr;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
