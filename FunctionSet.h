//
//  FunctionSet.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/9/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

// The FunctionSet class defines a “domain specific language” in which Programs
// of Individuals are written. It defines the names and parameter lists of the
// functions. Because LazyPredator uses “strongly typed genetic programming”
// (STGP) FunctionSet also defines a set of types associated with parameters and
// value of the functions in the set. Finally, “ephemeral constants”--randomized
// numeric leaf values--are defined here.
//
// TODO--except in trivial cases these functions will be API from some other
// software system. (TexSyn is the initial target.) FunctionSet needs to be
// flexible to allow connecting with external API.
//
// TODO--so far this is all just prototyping

// TODO note about terminology -- as in "minimum 'size' required to terminate
// subtree with this function at root" -- terminate in the sense of "terminate
// the top-down random program generation process" -- consider also that it
// could be phrased as "minimum depth of subtree..." better to use that?
// "Termination" applies more to the construction process, but I think we can
// describe it in terms of static program structure.

#pragma once
#include <map>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Sep 5 temporary experiments with std::any
#include <any>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <limits>
#include "Utilities.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
#include "Sandbox.h"
#else  // USE_STD_ANY
#endif // USE_STD_ANY
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO experimental September  3, 2020
// New model assuming types and functions are user-defined classes
// This also seems to be leaning toward a soup of zillions of small heap-
//     allocated instances strung together with shared_ptr<>

 
//    // TODO experimental
//    // Base type for all GP components
//    class NewGpObjectBase
//    {
//    public:
//        NewGpObjectBase() {}
//        NewGpObjectBase(const std::string& name) : name_(name) {}
//        virtual ~NewGpObjectBase() {}
//        const std::string& name() const { return name_; }
//    private:
//        std::string name_;
//    };
//
//    // TODO experimental
//    // Base type (or is it just "the type"?) for all GpValue wrappers
//    //
//    // TODO maybe store some indication (pointer to instance?, ID number?)
//    //      so we can ensure it was cast from and to the same type?
//    //
//    class NewGpValue/*Base*/ : public NewGpObjectBase
//    {
//    public:
//        NewGpValue(){}
//        // virtual ~NewGpValue() {}
//    //    template<typename T> T get() const { return static_cast<T>(thing_); }
//        template<typename T> std::shared_ptr<T> get() //const
//        {
//    //        return static_cast<std::shared_ptr<T>>(thing_);
//    //        return static_cast<T>(thing_);
//            return std::static_pointer_cast<T>(thing_);
//        }
//        // TODO just guessing here:
//    //    template<typename T> void set(T s) { thing_ = static_cast<void*>(s); }
//        template<typename T> void set(T s)
//        {
//            thing_ = static_cast<std::shared_ptr<void>>(s);
//        }
//    private:
//    //    void* thing_;
//        std::shared_ptr<void> thing_;
//    };
//
//    // TODO experimental
//    // Base type for all GpType, so for example a collection of user-defined classes
//    // derived from GpType could be pointers to this base class.
//    class NewGpTypeBase : public NewGpObjectBase
//    {
//    public:
//        NewGpTypeBase(){}
//        NewGpTypeBase(const std::string& name) : NewGpObjectBase(name) {}
//
//        // TODO Sep 4 make these virtual functions
//        // Does this type have an ephemeral generator? Default is no. Can be
//        // overridden by derived classes.
//        virtual bool hasEphemeralGenerator() const { return false; }
//        virtual NewGpValue generateEphemeralConstant()
//        {
//            assert(hasEphemeralGenerator());
//            return NewGpValue();
//        }
//
//    private:
//    };
//
//    // TODO experimental
//    // Base type for all GpFunction, so for example a collection of user-defined
//    // classes derived from GpFunction could be pointers to this base class.
//    class NewGpFunctionBase : public NewGpObjectBase
//    {
//    public:
//        NewGpFunctionBase(){}
//        NewGpFunctionBase(const std::string& name) : NewGpObjectBase(name) {}
//    private:
//    };

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// TODO experimental, purely an example

//    //class MyIntType : public NewGpTypeBase
//    //{
//    //public:
//    //    MyIntType(){}
//    //    MyIntType(const std::string& name) : NewGpTypeBase(name) {}
//    //    NewGpValue generateEphemeralConstant() override
//    //    {
//    //        NewGpValue v;
//    //        int* i = new(int);                    // TODO memory leak
//    //        *i = rs_.randomN(90) + 10;
//    //        v.set<int*>(i);
//    //        return v;
//    //    }
//    //private:
//    //    RandomSequence rs_;
//    //};
//    //
//    //class MyIntFuncAdd : public NewGpFunctionBase
//    //{
//    //public:
//    //    MyIntFuncAdd(){}
//    //    MyIntFuncAdd(const std::string& name) : NewGpFunctionBase(name) {}
//    //    // int eval(int i, int j) const { return i + j; }
//    //    int eval() const { return *a.get<int*>() + *b.get<int*>(); }
//    //private:
//    //    NewGpValue a;
//    //    NewGpValue b;
//    //};
//
//    class MyFloat01Type : public NewGpTypeBase
//    {
//    public:
//        typedef std::shared_ptr<float> SharedPointerToFloat;
//        MyFloat01Type(){}
//        MyFloat01Type(const std::string& name) : NewGpTypeBase(name) {}
//        NewGpValue generateEphemeralConstant() override
//        {
//            NewGpValue v;
//
//            auto f = std::make_shared<float>();
//            *f = rs_.frandom01();
//
//            auto q = static_cast<std::shared_ptr<void>>(f);
//
//            v.set<std::shared_ptr<float>>(f);
//
//            return v;
//        }
//        float get(NewGpValue v)
//        {
//            return **(v.get<SharedPointerToFloat>());
//        }
//    private:
//        RandomSequence rs_;
//    };


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES

// TODO Sep 6 temporary experiments with std::any
// old GpType and GpFunction classes updated to use std::any etc.

// TODO maybe add a GpObjectBase class for name(), etc.?

// Class to represent types in "strongly typed genetic programming".
class GpFunction;
class GpType
{
public:
    GpType(){}
    GpType(const std::string& name) : name_(name) {}
//        GpType(const std::string& name,
//    //           std::function<std::string()> ephemeral_generator)
//               std::function<std::any()> ephemeral_generator)
//            : name_(name), ephemeral_generator_(ephemeral_generator){}
    GpType(const std::string& name,
           std::function<std::any()> ephemeral_generator,
           std::function<std::string(std::any a)> to_string)
      : name_(name),
        ephemeral_generator_(ephemeral_generator),
        to_string_(to_string) {}
    
    const std::string& name() const { return name_; }
    
    // Does this type have an ephemeral generator?
    bool hasEphemeralGenerator() const { return bool(ephemeral_generator_); }
    // Generate an ephemeral constant.
    std::any generateEphemeralConstant() const
    {
        assert(hasEphemeralGenerator());
        return ephemeral_generator_();
    }

    const std::vector<GpFunction*>& functionsReturningThisType() const
    { return functions_returning_this_type_; }
    void addFunctionReturningThisType(GpFunction* gp_function_pointer)
    { functions_returning_this_type_.push_back(gp_function_pointer); }
    // Minimum "size" of tree returning this type from root;
    int minSizeToTerminate() const { return min_size_to_terminate_; }
    void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
    void print();
    bool valid() const
    {
        return ((!name().empty()) &&
                // TODO may be wrong given later changes to ephemeral constants:
                (!functionsReturningThisType().empty()) &&
                (minSizeToTerminate() < std::numeric_limits<int>::max()));
    }
    
    std::string to_string(std::any a) const { return to_string_(a); }

private:
    std::string name_;
    
//    // This should be templated to the c++ type
//    // TODO right now it is the type name as a string.
//    std::function<std::string()> ephemeral_generator_ = nullptr;
    
    std::function<std::any()> ephemeral_generator_ = nullptr;

    std::function<std::string(std::any a)> to_string_ = nullptr;

    
    // Collection of pointers to GpFunctions which return this type.
    std::vector<GpFunction*> functions_returning_this_type_;
    // Minimum "size" of tree returning this type from root;
    int min_size_to_terminate_ = std::numeric_limits<int>::max();
};

// Class to represent functions in "strongly typed genetic programming".
class GpFunction
{
public:
    GpFunction(){}
    GpFunction(const std::string& name,
               const std::string& return_type_name,
               const std::vector<std::string>& parameter_type_names)
      : name_(name),
        return_type_name_(return_type_name),
        parameter_type_names_(parameter_type_names) {}
    const std::string& name() const { return name_; }
    const std::string& returnTypeName() const { return return_type_name_; }
    GpType* returnType() const { return return_type_; }
    const std::vector<GpType*>& parameterTypes() const
        { return parameter_types_; }
    const std::vector<std::string>& parameterTypeNames() const
        { return parameter_type_names_; }
    void setReturnTypePointer(GpType* rtp) { return_type_ = rtp; }
    void setParameterTypePointers(const std::vector<GpType*>& vec_ptype_ptrs)
        { parameter_types_ = vec_ptype_ptrs; }
    // Does this function have parameter of its return type? (Can call itself?)
    bool recursive() const
    {
        bool r = false;
        for (auto& n : parameterTypeNames()) if(n == returnTypeName()) r = true;
        return r;
    }
    // Minimum "size" required to terminate subtree with this function at root.
    int minSizeToTerminate() const { return min_size_to_terminate_; }
    void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
    void print()
    {
        std::cout << "GpFunction: " << name() << ", return_type: ";
        std::cout << returnTypeName() << ", parameters: (";
        bool comma = false;
        for (int i = 0; i < parameterTypes().size(); i ++)
        {
            if (comma) std::cout << ", "; else comma = true;
            std::cout << parameterTypeNames().at(i);
        }
        std::cout << ")." << std::endl;
    }
private:
    std::string name_;
    std::string return_type_name_;
    GpType* return_type_ = nullptr;
    std::vector<std::string> parameter_type_names_;
    std::vector<GpType*> parameter_types_;
    int min_size_to_terminate_ = std::numeric_limits<int>::max();
};

// Down here because it requires both GpType and GpFunction to be defined.
inline void GpType::print()
{
    std::cout << "GpType: " << name();
    std::cout << ", min size to terminate: " << minSizeToTerminate();
    std::cout << ", " << (hasEphemeralGenerator() ? "has" : "no");
    std::cout << " ephemeral generator";
    std::cout << ", " << (to_string_ ? "has" : "no");
    std::cout << " to_string";
    if (functions_returning_this_type_.size() > 0)
    {
        bool first = true;
        std::cout << ", functions returning this type: ";
        for (auto& f : functions_returning_this_type_)
        {
            if (first) first = false; else std::cout << ", ";
            std::cout << f->name();
        }
    }
    std::cout << "." << std::endl;
}


#else  // USE_STD_ANY_WITH_OLD_CLASSES
typedef Sep5GpTypeBase GpType;
typedef Sep5GpFunctionBase GpFunction;
#endif // USE_STD_ANY_WITH_OLD_CLASSES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#else  // USE_STD_ANY

//    // Class to represent types in "strongly typed genetic programming".
//    class GpFunction;
//    class GpType
//    {
//    public:
//        GpType(){}
//        GpType(const std::string& name) : name_(name) {}
//        GpType(const std::string& name,
//               std::function<std::string()> ephemeral_generator)
//          : name_(name), ephemeral_generator_(ephemeral_generator){}
//        const std::string& name() const { return name_; }
//        const std::function<std::string()> ephemeralGenerator() const
//            { return ephemeral_generator_; }
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO Sep 6 temporary experiments with std::any
//    #ifdef USE_STD_ANY
//        // TODO Sep 5 temporary experiments with std::any
//        GpType(const std::string& name,
//    //           std::function<std::string()> ephemeral_generator,
//               std::function<std::any()> eg)
//        : name_(name), ephemeral_generator_(ephemeral_generator), eg_(eg) {}
//        bool hasEG() const { return bool(eg_); }
//        std::any gEC() const { return eg_(); } // can this be non-const for rs()?
//    private:
//        std::function<std::any()> eg_ = nullptr;
//    public:
//    #else  // USE_STD_ANY
//    #endif // USE_STD_ANY
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        const std::vector<GpFunction*>& functionsReturningThisType() const
//            { return functions_returning_this_type_; }
//        void addFunctionReturningThisType(GpFunction* gp_function_pointer)
//            { functions_returning_this_type_.push_back(gp_function_pointer); }
//        // Minimum "size" of tree returning this type from root;
//        int minSizeToTerminate() const { return min_size_to_terminate_; }
//        void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
//        void print();
//        bool valid() const
//        {
//            return ((!name().empty()) &&
//                    // TODO may be wrong given later changes to ephemeral constants:
//                    (!functionsReturningThisType().empty()) &&
//                    (minSizeToTerminate() < std::numeric_limits<int>::max()));
//        }
//    private:
//        std::string name_;
//        // This should be templated to the c++ type
//        // TODO right now it is the type name as a string.
//        std::function<std::string()> ephemeral_generator_ = nullptr;
//        // Collection of pointers to GpFunctions which return this type.
//        std::vector<GpFunction*> functions_returning_this_type_;
//        // Minimum "size" of tree returning this type from root;
//        int min_size_to_terminate_ = std::numeric_limits<int>::max();
//    };
//
//    // Class to represent functions in "strongly typed genetic programming".
//    class GpFunction
//    {
//    public:
//        GpFunction(){}
//        GpFunction(const std::string& name,
//                   const std::string& return_type_name,
//                   const std::vector<std::string>& parameter_type_names)
//          : name_(name),
//            return_type_name_(return_type_name),
//            parameter_type_names_(parameter_type_names) {}
//        const std::string& name() const { return name_; }
//        const std::string& returnTypeName() const { return return_type_name_; }
//        GpType* returnType() const { return return_type_; }
//        const std::vector<GpType*>& parameterTypes() const
//            { return parameter_types_; }
//        const std::vector<std::string>& parameterTypeNames() const
//            { return parameter_type_names_; }
//        void setReturnTypePointer(GpType* rtp) { return_type_ = rtp; }
//        void setParameterTypePointers(const std::vector<GpType*>& vec_ptype_ptrs)
//            { parameter_types_ = vec_ptype_ptrs; }
//        // Does this function have parameter of its return type? (Can call itself?)
//        bool recursive() const
//        {
//            bool r = false;
//            for (auto& n : parameterTypeNames()) if(n == returnTypeName()) r = true;
//            return r;
//        }
//        // Minimum "size" required to terminate subtree with this function at root.
//        int minSizeToTerminate() const { return min_size_to_terminate_; }
//        void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
//        void print()
//        {
//            std::cout << "GpFunction: " << name() << ", return_type: ";
//            std::cout << returnTypeName() << ", parameters: (";
//            bool comma = false;
//            for (int i = 0; i < parameterTypes().size(); i ++)
//            {
//                if (comma) std::cout << ", "; else comma = true;
//                std::cout << parameterTypeNames().at(i);
//            }
//            std::cout << ")." << std::endl;
//        }
//    private:
//        std::string name_;
//        std::string return_type_name_;
//        GpType* return_type_ = nullptr;
//        std::vector<std::string> parameter_type_names_;
//        std::vector<GpType*> parameter_types_;
//        int min_size_to_terminate_ = std::numeric_limits<int>::max();
//    };
//
//    // Down here because it requires both GpType and GpFunction to be defined.
//    inline void GpType::print()
//    {
//        std::cout << "GpType: " << name();
//        std::cout << ", min size to terminate: " << minSizeToTerminate();
//        std::cout << ", " << (ephemeralGenerator() ? "has" : "no");
//        std::cout << " ephemeral generator";
//        if (functions_returning_this_type_.size() > 0)
//        {
//            bool first = true;
//            std::cout << ", functions returning this type: ";
//            for (auto& f : functions_returning_this_type_)
//            {
//                if (first) first = false; else std::cout << ", ";
//                std::cout << f->name();
//            }
//        }
//        std::cout << "." << std::endl;
//    }

#endif // USE_STD_ANY
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // TODO experimental August 30, 2020
//    template<typename T>
//    class TestGpType : public GpType
//    {
//    public:
//        TestGpType(){}
//        TestGpType(const std::string& name) : GpType(name){}
//        TestGpType(const std::string& name, std::function<T()> ephemeral_generator)
//          : GpType(name), ephemeral_generator_(ephemeral_generator){}
//        bool hasEphemeralGenerator() const { return bool(ephemeral_generator_); }
//        T generateEphemeralConstant() const { return ephemeral_generator_(); }
//    private:
//        std::function<T()> ephemeral_generator_ = nullptr;
//    };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// GpTree: a "program tree", an "abstract syntax tree" ("AST"), to represent a
// composition of GpFunction(s) and GpType(s). Each GpTree instance contains a
// vector of subtrees, for each parameter of the function at the root, and so
// on recursively, so as to contain the whole tree.
class GpTree
{
public:
    // Default costructor.
    GpTree(){}
    // Reference to vector of subtrees, const or not.
    std::vector<GpTree>& subtrees() { return subtrees_; }
    const std::vector<GpTree>& subtrees() const { return subtrees_; }
    // Get reference to i-th subtree. Like: subtrees().at(i)
    GpTree& getSubtree(int i) { return subtrees().at(i); }
    const GpTree& getSubtree(int i) const { return subtrees().at(i); }
    // Get/set reference to GpFunction object at root of this tree.
    const GpFunction& getFunction() const { return *root_function_; }
    void setFunction(const GpFunction& function) { root_function_ = &function; }
    
    // TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES
    void setType(const GpType& gp_type) { root_type_ = &gp_type; }
#else  // USE_STD_ANY_WITH_OLD_CLASSES
#endif // USE_STD_ANY_WITH_OLD_CLASSES

    
    // Add (allocate) "count" new subtrees. Should only be called once.
    // TODO maybe instead of "add" call it "declare" or "init"?
    void addSubtrees(size_t count)
    {
        assert("call addSubtrees() only once" && subtrees().size() == 0);
        for (int i = 0; i < count; i++) addSubtree();
    }
    // Count tokens in tree (functions or leaves/constants).
    int size() const
    {
        int count = 1;
        for (auto& subtree : subtrees()) count += subtree.size();
        return count;
    }
    // Get/set leaf value, an instantiated "ephemeral constant"
    // TODO use of string is just a "mock" until types are templated.
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
    
    // TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES
    std::string getLeafValueAsString() const
    {
        return root_type_->to_string(leaf_value_);
    }
#else  // USE_STD_ANY_WITH_OLD_CLASSES
    std::string getLeafValueAsString() const
    {
        return std::to_string(std::any_cast<int>(leaf_value_));
    }
#endif // USE_STD_ANY_WITH_OLD_CLASSES

    void setLeafValue(std::any value) { leaf_value_ = value; }
#else  // USE_STD_ANY
    const std::string& getLeafValue() const { return leaf_value_; }
    void setLeafValue(const std::string& value) { leaf_value_ = value; }
#endif // USE_STD_ANY
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // Convert this GpTree to "source code" format as, a string. It is either a
    // single constant "leaf" value, or a function name then a parenthesized,
    // comma separated list of parameter trees.
    std::string to_string() const
    {
        std::string s;
        if (root_function_)
        {
            s += getFunction().name() + "(";
            bool comma = false;
            for (auto& subtree : subtrees())
            {
                if (comma) s += ", "; else comma = true;
                s += subtree.to_string();
            }
            s += ")";
        }
        else
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
            s += getLeafValueAsString();
#else  // USE_STD_ANY
            s += getLeafValue();
#endif // USE_STD_ANY
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
        return s;
    }
    std::string id() const { return id_; }      // TODO for debugging only.
    void setId(std::string s) { id_ = s; }      // TODO for debugging only.
private:
    // Add (allocate) one subtree. addSubtrees() is external API.
    void addSubtree() { subtrees_.push_back({}); }
    // Each GpTree (sub)root will have a function object or a leaf value.
    const GpFunction* root_function_ = nullptr;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES
    const GpType* root_type_ = nullptr;
#else  // USE_STD_ANY_WITH_OLD_CLASSES
#endif // USE_STD_ANY_WITH_OLD_CLASSES

    
    // TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
    std::any leaf_value_;
#else  // USE_STD_ANY
    std::string leaf_value_;  // TODO mocked as string until templates.
#endif // USE_STD_ANY
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::vector<GpTree> subtrees_;
    std::string id_;                            // TODO for debugging only.
};

class FunctionSet
{
public:
    FunctionSet(){}
    // New constructor using vectors of GpType and GpFunction.
    // TODO eventually this needs to be rewritten to be smaller.
    FunctionSet(const std::vector<GpType>& type_specs,
                const std::vector<GpFunction>& function_specs)
    {
        // Process each GpType specifications, make local copy to modify.
        for (GpType gp_type : type_specs)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
//            if (gp_type.hasEG()) gp_type.setMinSizeToTerminate(1);
            bool has_eg = gp_type.hasEphemeralGenerator();
            if (has_eg) gp_type.setMinSizeToTerminate(1);
#else  // USE_STD_ANY
            // If ephemeral generator provided, type terminates with size 1.
            auto& eg = gp_type.ephemeralGenerator();
            if (eg) gp_type.setMinSizeToTerminate(1);
#endif // USE_STD_ANY
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Insert updated GpType into by-name map. (Copied again into map.)
            addGpType(gp_type);
        }
        // Process each of the GpFunction specifications (copy then modify)
        for (GpFunction func : function_specs)
        {
            // Character string name of this function's return type.
            std::string rtn = func.returnTypeName();
            // Connect type names to GpType object
            func.setReturnTypePointer(lookupGpTypeByName(rtn));
            std::vector<GpType*> parameter_types;
            for (auto& parameter_name : func.parameterTypeNames())
                {parameter_types.push_back(lookupGpTypeByName(parameter_name));}
            func.setParameterTypePointers(parameter_types);
            // If this function does not call its own type (eg Uniform) it can
            // be used to terminate a subtree of that type. Record in GpType.
            if (!func.recursive())
            {
                // std::cout << "NOT recursive: " << func.returnTypeName();
                GpType& rt = *lookupGpTypeByName(rtn);
                // TODO this is "min size to terminate type" ASSUMING all
                // parameters are ephemeral constants. Make more general.
                int mstt = 1 + int(func.parameterTypeNames().size());
                if (mstt < rt.minSizeToTerminate())
                    rt.setMinSizeToTerminate(mstt);
                // std::cout << " min size to terminate: " << mstt << std::endl;
            }
            // Copy once more into name-to-GpFunction-object map.
            addGpFunction(func);
        }
        // Build per-GpType collections of GpFunctions returning that type.
        for (auto& [name, gp_function] : nameToGpFunctionMap())
        {
            GpType& gp_type = *gp_function.returnType();
            gp_type.addFunctionReturningThisType(&gp_function);
        }
        // Set minSizeToTerminate() for each GpFunction.
        for (auto& [name, gp_function] : nameToGpFunctionMap())
        {
            int min_size = minSizeToTerminateFunction(gp_function);
            gp_function.setMinSizeToTerminate(min_size);
        }
    }

    // TODO just for debugging
    bool dp = false;
    // TODO just for debugging
    int dp_depth = 0;
    // TODO just for debugging
    void dp_prefix()
    {
        if (dp)
        {
            std::string prefix;
            for (int i = 0; i < dp_depth + 1; i++) prefix += "| ";
            std::cout << prefix;
        }
    }
    
    // What is the minimum "size" required to terminate a program subtree with
    // the given function at the root? At FunctionSet construction time, this
    // computes the number which is then stored on each GpFunction instance.
    int minSizeToTerminateFunction(const GpFunction& gp_function)
    {
        // We need 1 for the function name itself (or an ephemeral constant).
        int size = 1;
        // Then loop over all parameter types.
        for (auto& parameter_gp_type : gp_function.parameterTypes())
        {
            size += parameter_gp_type->minSizeToTerminate();
        }
        return size;
    }

    // Randomly select a function in this set that returns the given type and
    // can be implemented in a subtree no larger than max_size. Returns nullptr
    // if none found.
    GpFunction* randomFunctionOfTypeInSize(int max_size,
                                           const GpType& return_type)
    {
        std::vector<GpFunction*> ok;
        for (auto& gp_function : return_type.functionsReturningThisType())
        {
            int ms = gp_function->minSizeToTerminate();
            if (max_size >= ms) { ok.push_back(gp_function); }
        }
        if (ok.empty() && dp)
        {
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type.name());
        }
        return (ok.empty() ? nullptr : ok.at(rs().randomN(ok.size())));
    }

    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // nodes (function calls or constants) in the resulting program. The program
    // returns a value of "return_type" from its root.
    void makeRandomProgram(int max_size,
                           const GpType& return_type,
                           int& output_actual_size,
                           std::string& source_code,
                           GpTree& gp_tree)
    {
        if (dp)
        {
            dp_prefix();
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type.name());
            dp_prefix(); debugPrint(source_code);
            dp_prefix(); std::cout << std::endl;
        }
        // Find all function whose value is return_type, for which a subtree can
        // be constructed in max_size or fewer nodes, and select on randomly.
        GpFunction* rf = randomFunctionOfTypeInSize(max_size, return_type);
        if (rf)
        {
            // If found, recurse on a subtree with that function at the root.
            makeRandomProgramRoot(max_size, return_type, *rf,
                                  output_actual_size, source_code, gp_tree);
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO Sep 6 temporary experiments with std::any
#ifdef USE_STD_ANY
//        else if (return_type.hasEG())
        else if (return_type.hasEphemeralGenerator())
        {
            // If no function found, but this type has an ephemeral generator,
            // use it to generate a leaf constant, ending recursion.
            output_actual_size++;
            // TODO IMPORTANT recall this use of strings is just a "mock"
            //      until I get around to templating to the actual c++ type.
            // TODO should pass rs() into the generator for repeatability.
            
//            std::string constant = return_type.ephemeralGenerator()();
//            source_code += constant;
//            std::any leaf_value = return_type.gEC();
            std::any leaf_value = return_type.generateEphemeralConstant();
//            source_code += std::any_cast<int>(leaf_value);
            source_code += return_type.to_string(leaf_value);
            gp_tree.setLeafValue(leaf_value);
            
            // TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES
            gp_tree.setType(return_type);
#else  // USE_STD_ANY_WITH_OLD_CLASSES
#endif // USE_STD_ANY_WITH_OLD_CLASSES

        }
#else  // USE_STD_ANY
        else if (return_type.ephemeralGenerator())
        {
            // If no function found, but this type has an ephemeral generator,
            // use it to generate a leaf constant, ending recursion.
            output_actual_size++;
            // TODO IMPORTANT recall this use of strings is just a "mock"
            //      until I get around to templating to the actual c++ type.
            // TODO should pass rs() into the generator for repeatability.
            std::string constant = return_type.ephemeralGenerator()();
            source_code += constant;
            gp_tree.setLeafValue(constant);
        }
#endif // USE_STD_ANY
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else
        {
            // TODO better way to signal this FunctionSet specification error?
            std::cout << "No function or epheneral constant found for GpType ";
            std::cout << return_type.name() << " remaining size: " << max_size;
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        if (dp)
        {
            dp_prefix();
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        }
    }

    // Overload to allow passing return_type_name as a string from top level.
    void makeRandomProgram(int max_size,
                           const std::string& return_type_name,
                           int& output_actual_size,
                           std::string& source_code,
                           GpTree& gp_tree)
    {
        makeRandomProgram(max_size,
                          *lookupGpTypeByName(return_type_name),
                          output_actual_size,
                          source_code,
                          gp_tree);
    }

    void makeRandomProgramRoot(int max_size,
                               const GpType& return_type,
                               const GpFunction& root_function,
                               int& output_actual_size,
                           std::string& source_code,
                           GpTree& gp_tree)
    {
        if (dp)
        {
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type.name());
            dp_prefix(); debugPrint(root_function.name());
            dp_prefix(); debugPrint(root_function.parameterTypes().size());
        }
        output_actual_size++;  // for "function_name" (or epheneral) itself
        int size_used = 0;
        int count = int(root_function.parameterTypes().size());
        source_code += root_function.name() + "(";  // TODO log
        bool first = true;   // TODO log
        // Set root function in given GpTree object
        gp_tree.setFunction(root_function);

        // TODO Sep 7 temporary experiments with std::any
#ifdef USE_STD_ANY_WITH_OLD_CLASSES
        gp_tree.setType(return_type);
#else  // USE_STD_ANY_WITH_OLD_CLASSES
#endif // USE_STD_ANY_WITH_OLD_CLASSES

        // For each parameter of root, add/allocate a subtree in the GpTree.
        // Important this happen first so no iterators are invalidated later.
        gp_tree.addSubtrees(root_function.parameterTypes().size());
        // TODO do this in a cleaner way after it is working
        int subtree_index = 0;
        // Loop over each parameter of "function_name" generating a subtree.
        for (auto& parameter_type : root_function.parameterTypes())
        {
            if (dp)
            {
                dp_prefix(); std::cout << "((((((((((((((((((";
                std::cout << " param=" << root_function.parameterTypes().size() - count;
                std::cout << " type=" << parameter_type;
                std::cout << std::endl;
            }
            if (first) first = false; else source_code += ", ";  // TODO log
            int fair_share = (max_size - (1 + size_used)) / count;
            int min_size_for_type = parameter_type->minSizeToTerminate();
            int subtree_max_size = std::max(fair_share, min_size_for_type);
            if (dp) { dp_prefix(); debugPrint(subtree_max_size); }
            int subtree_actual_size = 0;
            std::string subtree_source;
            dp_depth++;
            makeRandomProgram(subtree_max_size, *parameter_type,
                              subtree_actual_size, subtree_source,
                              gp_tree.subtrees().at(subtree_index));
            // TODO do this in a cleaner way after it is working
            subtree_index++;
            dp_depth--;
            output_actual_size += subtree_actual_size;
            size_used += subtree_actual_size;
            source_code += subtree_source;
            if (dp)
            {
                dp_prefix(); debugPrint(subtree_max_size);
                dp_prefix(); debugPrint(subtree_actual_size);
                dp_prefix(); debugPrint(subtree_source);
                dp_prefix(); debugPrint(count);
            }
            count--;
            if (dp)
            {
                dp_prefix(); std::cout << "))))))))))))))))))" << std::endl;
            }
        }
        source_code += ")";
    }

    void print()
    {
        for (auto& [n, t] : nameToGpTypeMap()) t.print();
        for (auto& [n, f] : nameToGpFunctionMap()) f.print();
    }
    
    // Accessor for RandomSequence, perhaps only needed for testing?
    RandomSequence& rs() { return rs_; }
    
    // TODO maybe there should be const versions of these for "observers" like
    // UnitTests.
    // TODO should these be private?
    // TODO better to return pointer or reference?
    // Lookup pointer to GpType/GpFunction object.
    GpType* lookupGpTypeByName(const std::string& name) //const
    {
        auto it = name_to_gp_type_.find(name);
        assert("unknown type" && (it != name_to_gp_type_.end()));
        return &(it->second);
    }
    GpFunction* lookupGpFunctionByName(const std::string& name) //const
    {
        auto it = name_to_gp_function_.find(name);
        assert("unknown function" && (it != name_to_gp_function_.end()));
        return &(it->second);
    }
    // Add new GpType/GpFunction to FunctionSet, stored in a name-to-object map.
    void addGpType(GpType& type) { name_to_gp_type_[type.name()] = type; }
    void addGpFunction(GpFunction& f) { name_to_gp_function_[f.name()] = f; }
    // Get reference to name-to-object maps of all GpType/GpFunction objects
    std::map<std::string, GpType>& nameToGpTypeMap()
        { return name_to_gp_type_; }
    std::map<std::string, GpFunction>& nameToGpFunctionMap()
        { return name_to_gp_function_; }
private:
    // These maps are used both to store the GpType and GpFunction objects,
    // plus to look up those objects from their character string names.
    std::map<std::string, GpType> name_to_gp_type_;
    std::map<std::string, GpFunction> name_to_gp_function_;
    RandomSequence rs_;
};
