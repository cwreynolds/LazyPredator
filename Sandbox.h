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
    // Just a shorthand convenience function. (Could even be to<int>(a))
    template<typename T> T anyTo(std::any a) const {return std::any_cast<T>(a);}
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
    
    // TODO late Sep 5, virtual for changing a leaf value to string
    virtual std::string to_string(std::any a) { return "..."; }
    
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
//    Sep5Float01Type(const std::string& name) : Sep5GpTypeBase(name) {}
    Sep5Float01Type() : Sep5GpTypeBase("Float01") {}
    bool hasEphemeralGenerator() const override { return true; }
    std::any generateEphemeralConstant() override
    {
        return std::any(rs().frandom01());
    }
    // TODO late Sep 5, virtual for changing a leaf value to std::string.
    std::string to_string(std::any a) override
    {
        return std::to_string(anyTo<float>(a));
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

// TODO -- OH! shouldn't we supply "return_type_name" and "parameter_type_names"
//         as inline constants like "name"?
class Sep5Vec2Function : public Sep5GpFunctionBase
{
public:
    Sep5Vec2Function(const std::string& return_type_name,
                     const std::vector<std::string>& parameter_type_names)
      : Sep5GpFunctionBase("Vec2", return_type_name, parameter_type_names) {}

    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(dummyVec2(anyTo<float>(dummyTreeNodeSubtree(0)),
                                  anyTo<float>(dummyTreeNodeSubtree(1))));
    }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Sep 6 temporary experiments with std::any
//
// Corresponds to this, in main.cpp
//
//        // lets call this FunctionSet test_tree_eval
//        std::string root_type = "Float";
//        FunctionSet fs =
//        {
//            {
//                {"Int", [](){ return std::to_string(int(rand() % 10)); }},
//                {"Float", [](){ return std::to_string(frandom01()); }}
//            },
//            {
//                {"AddInt", "Int", {"Int", "Int"}},
//                {"AddFloat", "Float", {"Float", "Float"}},
//                {"Floor", "Int", {"Float"}},
//                {"Sqrt", "Float", {"Int"}},
//                {"Mult", "Float", {"Float", "Int"}}
//            }
//        };

// TODO if this is how we end up doing it, this could be a macro taking three
// parameters: class name, string name, and a function body.

// TODO -- OH! shouldn't we supply "return_type_name" and "parameter_type_names"
//         as inline constants like "name"?

class Sep5AddIntFunction : public Sep5GpFunctionBase
{
public:
    Sep5AddIntFunction() : Sep5GpFunctionBase("AddInt",
                                              "Int",
                                              {"Int", "Int"}) {}
    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(anyTo<int>(dummyTreeNodeSubtree(0)) +
                        anyTo<int>(dummyTreeNodeSubtree(1)));
    }
};

class Sep5AddFloatFunction : public Sep5GpFunctionBase
{
public:
    Sep5AddFloatFunction() :
        Sep5GpFunctionBase("AddFloat", "Float", {"Float", "Float"}) {}
    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(anyTo<float>(dummyTreeNodeSubtree(0)) +
                        anyTo<float>(dummyTreeNodeSubtree(1)));
    }
};

class Sep5FloorFunction : public Sep5GpFunctionBase
{
public:
    Sep5FloorFunction() : Sep5GpFunctionBase("Floor", "Int", {"Float"}) {}
    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(std::floor(anyTo<float>(dummyTreeNodeSubtree(0))));
    }
};

class Sep5SqrtFunction : public Sep5GpFunctionBase
{
public:
    Sep5SqrtFunction() : Sep5GpFunctionBase("Sqrt", "Float", {"Int"}) {}
    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(std::sqrt(anyTo<float>(dummyTreeNodeSubtree(0))));
    }
};

class Sep5MultFunction : public Sep5GpFunctionBase
{
public:
    Sep5MultFunction() :
        Sep5GpFunctionBase("Mult", "Float", {"Float", "Int"}) {}
    // TODO just trying to rough out how an actual function call would look.
    std::any evalTreeNode(/* GpTree */) override
    {
        return std::any(anyTo<float>(dummyTreeNodeSubtree(0)) *
                        anyTo<int>(dummyTreeNodeSubtree(1)));
    }
};

class Sep5IntType : public Sep5GpTypeBase
{
public:
    Sep5IntType() : Sep5GpTypeBase("Int") {}
    bool hasEphemeralGenerator() const override { return true; }
    std::any generateEphemeralConstant() override
    {
        return std::any(int(rand() % 10));
    }
    // TODO late Sep 5, virtual for changing a leaf value to std::string.
    std::string to_string(std::any a) override
    {
        return std::to_string(anyTo<int>(a));
    }
};

class Sep5FloatType : public Sep5GpTypeBase
{
public:
    Sep5FloatType() : Sep5GpTypeBase("Float") {}
    bool hasEphemeralGenerator() const override { return true; }
    std::any generateEphemeralConstant() override
    {
        return std::any(frandom01());
    }
    // TODO late Sep 5, virtual for changing a leaf value to std::string.
    std::string to_string(std::any a) override
    {
        return std::to_string(anyTo<float>(a));
    }
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
