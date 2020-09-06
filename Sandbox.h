//
//  Sandbox.hpp
//  LazyPredator
//
//  Created by Craig Reynolds on 9/5/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

//  This is a temporary file, just for some experiments

#pragma once
#include "Utilities.h"
#include <any>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO starting again
//     September 5, 2020
//     after intial experiments with std::any and std::variant


//    static FunctionSet& tiny()
//    {
//        static FunctionSet tiny =
//        {
//            {
//                {"Float_01", [](){ return frandom01() < 0.5 ? "0" : "1"; }},
//                {"Texture"},
//                {"Vec2"}
//            },
//            {
//                {"Vec2", "Vec2", {"Float_01", "Float_01"}},
//                {"Uniform", "Texture", {"Float_01", "Float_01", "Float_01"}},
//                {"Spot", "Texture",
//                    {"Vec2", "Float_01", "Texture", "Float_01", "Texture"}},
//                {"Add", "Texture", {"Texture", "Texture"}},
//                {"Blur", "Texture", {"Float_01", "Texture"}},
//                {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}}
//            }
//        };
//        return tiny;
//    }

// TODO experimental
// Base type for all GP components
class Sep5GpObjectBase
{
public:
    Sep5GpObjectBase() {}
    Sep5GpObjectBase(const std::string& name) : name_(name) {}
    virtual ~Sep5GpObjectBase() {}
    const std::string& name() const { return name_; }
    float anyToFloat(std::any a) { return std::any_cast<float>(a); }
private:
    std::string name_;
};

// TODO experimental
// Base type for all GpType, so for example a collection of user-defined classes
// derived from GpType could be pointers to this base class.
class Sep5GpTypeBase : public Sep5GpObjectBase
{
public:
    Sep5GpTypeBase(){}
    Sep5GpTypeBase(const std::string& name) : Sep5GpObjectBase(name) {}
    
    // Does this type have an ephemeral generator? Default is no. Can be
    // overridden by derived classes.
    virtual bool hasEphemeralGenerator() const { return false; }
    virtual std::any generateEphemeralConstant()
    {
        assert(hasEphemeralGenerator());
        return std::any();
    }
    RandomSequence& rs() { return rs_; }
private:
    RandomSequence rs_;
};

// TODO experimental
// Base type for all GpFunction, so for example a collection of user-defined
// classes derived from GpFunction could be pointers to this base class.
class Sep5GpFunctionBase : public Sep5GpObjectBase
{
public:
    Sep5GpFunctionBase(){}
    //Sep5GpFunctionBase(const std::string& name) : Sep5GpObjectBase(name) {}
    
    Sep5GpFunctionBase(const std::string& name,
                       const std::string& return_type_name,
                       const std::vector<std::string>& parameter_type_names)
      : name_(name),
        return_type_name_(return_type_name),
        parameter_type_names_(parameter_type_names) {}

    virtual std::any evalTreeNode(/* GpTree */) { return std::any(); }

private:
    std::string name_;
    std::string return_type_name_;
    // GpType* return_type_ = nullptr;
    std::vector<std::string> parameter_type_names_;
    // std::vector<GpType*> parameter_types_;
    int min_size_to_terminate_ = std::numeric_limits<int>::max();
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Float01 type
class Sep5Float01Type : public Sep5GpTypeBase
{
public:
    Sep5Float01Type(const std::string& name) : Sep5GpTypeBase(name) {}
    bool hasEphemeralGenerator() const override { return true; }
    std::any generateEphemeralConstant() override
    {
        return std::any(rs().frandom01());
    }
};

// Texture type
class Sep5TextureType : public Sep5GpTypeBase
{
public:
    Sep5TextureType() : Sep5GpTypeBase("Texture") {}
};

// Vec2 type
class Sep5Vec2Type : public Sep5GpTypeBase
{
public:
    Sep5Vec2Type() : Sep5GpTypeBase("Vec2") {}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//                {"Vec2", "Vec2", {"Float_01", "Float_01"}},

class dummyVec2
{
public:
    // Constructors
    dummyVec2() {}
    dummyVec2(float x, float y) : x_(x), y_(y) {}
private:
    float x_ = 0;
    float y_ = 0;
};

inline std::any dummyTreeNodeSubtree(int i) { return std::any(); }

class Sep5Vec2Function : public Sep5GpFunctionBase
{
public:
    Sep5Vec2Function(const std::string& return_type_name,
                     const std::vector<std::string>& parameter_type_names)
      : Sep5GpFunctionBase("Vec2", return_type_name, parameter_type_names) {}

    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(dummyVec2(anyToFloat(dummyTreeNodeSubtree(0)),
                                  anyToFloat(dummyTreeNodeSubtree(1))));
    }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
