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
#include <limits>
#include "Utilities.h"

// Class to represent types in "strongly typed genetic programming".
class GpFunction;
class GpType
{
public:
    GpType(){}
    GpType(const std::string& name) : name_(name) {}
    GpType(const std::string& name,
           std::function<std::string()> ephemeral_generator)
      : name_(name), ephemeral_generator_(ephemeral_generator){}
    const std::string& name() const { return name_; }
    const std::function<std::string()> ephemeralGenerator() const
        { return ephemeral_generator_; }
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
                (!functionsReturningThisType().empty()) &&
                (minSizeToTerminate() < std::numeric_limits<int>::max()));
    }
private:
    std::string name_;
    // This should be templated to the c++ type
    // TODO right now it is the type name as a string.
    std::function<std::string()> ephemeral_generator_ = nullptr;
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
        std::cout << "GpFunction: " << name() << ", return_type: " <<
            returnTypeName() << "(" <<  returnType() << "), parameters: (";
        bool comma = false;
        for (int i = 0; i < parameterTypes().size(); i ++)
        {
            if (comma) std::cout << ", "; else comma = true;
            std::cout << parameterTypeNames().at(i) << "(" <<
                parameterTypes().at(i) << ")";
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
    if (ephemeralGenerator()) std::cout << ", has ephemeral generator";
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GpTree: a "program tree", an "abstract syntax tree" ("AST"), to represent a
// composition of GpFunction(s) and GpType(s). Each GpTree instance is a "node"
// a "subtree root". It contains a vector of subtrees, each of type GpType.
//
// TODO PROTOTYPE
//

class GpTree
{
public:
    GpTree(){}
    
    const std::vector<GpTree>& subtrees() const { return subtrees_; }
    
    GpTree& addSubtree()
    {
        subtrees_.push_back({});
        return subtrees_.back();
    }
    
    std::string id() const { return id_; }  // TODO for debugging
    void setId(std::string s) { id_ = s; }  // TODO for debugging

private:
    std::vector<GpTree> subtrees_;
    
    std::string id_;  // TODO for debugging
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
            // If ephemeral generator provided, type terminates with size 1.
            auto& eg = gp_type.ephemeralGenerator();
            if (eg) gp_type.setMinSizeToTerminate(1);
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
        for (auto& pair : nameToGpFunctionMap())
        {
            GpFunction* f = &pair.second;
            GpType* t = f->returnType();
            t->addFunctionReturningThisType(f);
        }
        // Set minSizeToTerminate() for each GpFunction.
        for (auto& pair : nameToGpFunctionMap())
        {
            GpFunction& f = pair.second;
            f.setMinSizeToTerminate(minSizeToTerminateFunction(f));
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
    // can be implemented in a subtree no larger than max_size.
    GpFunction& randomFunctionOfTypeInSize(int max_size,
                                           const GpType& return_type)
    {
        std::vector<GpFunction*> fit_size;
        for (auto& gp_function : return_type.functionsReturningThisType())
        {
            int ms = gp_function->minSizeToTerminate();
            if (max_size >= ms) { fit_size.push_back(gp_function); }
        }
        if (fit_size.empty() && dp)
        {
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type.name());
        }
        assert(!fit_size.empty());
        return *(fit_size.at(rs_.nextInt() % fit_size.size()));
    }

    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // nodes (function calls or constants) in the resulting program. The program
    // returns a value of "return_type" from its root.
    void makeRandomProgram(int max_size,
                           const GpType& return_type,
                           int& output_actual_size,
                           std::string& source_code)
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
        // TODO, this is left-over confusion from initial protype which did not
        // differentiate well between types and functions, overloaded functions
        // with ephemeral generators, and used strings as IDs for both. Trying
        // to prototype a fix.
        //
        // TODO comment in DynaList: "can we handle the case where a `GpType`
        // has an ephemeral generator **and** there is a non-recursive function
        // that returns that type?" Here we give priority to EG if it exists.
        //
        if (return_type.ephemeralGenerator())
        {
            output_actual_size++;
            source_code += return_type.ephemeralGenerator()();
            // TODO -- to be removed -- here just to retain original random
            // sequence to preserve test case reproducability
            rs().frandom01();
        }
        else
        {
            GpFunction& rf = randomFunctionOfTypeInSize(max_size, return_type);
            makeRandomProgramRoot(max_size, return_type, rf,
                                  output_actual_size, source_code);
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
                           std::string& source_code)
    {
        makeRandomProgram(max_size,
                          *lookupGpTypeByName(return_type_name),
                          output_actual_size,
                          source_code);
    }

    void makeRandomProgramRoot(int max_size,
                               const GpType& return_type,
                               const GpFunction& root_function,
                               int& output_actual_size,
                               std::string& source_code)
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
                              subtree_actual_size, subtree_source);
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

    void printSet()
    {
        for (auto& pair : nameToGpTypeMap())
        {
            GpType& gp_type = pair.second;
            std::cout << "type: " << gp_type.name() << " " << &gp_type;
            std::cout << std::endl;
        }
        for (auto& pair : nameToGpFunctionMap())
        {
            bool first = true;
            GpFunction& gp_function = pair.second;
            std::cout << "function: " << gp_function.name() << ", returns: ";
            std::cout << gp_function.returnTypeName() << ", parameters: (";
            for (auto& pt : gp_function.parameterTypeNames())
            {
                if (first) first = false; else std::cout << ", ";
                std::cout << pt;
            }
            std::cout << ")" << std::endl;
        }
        for (auto& pair : nameToGpTypeMap()) pair.second.print();
        for (auto& pair : nameToGpFunctionMap()) pair.second.print();
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
