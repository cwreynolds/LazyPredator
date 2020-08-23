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

#pragma once
#include <map>
#include <limits>
#include "Utilities.h"

// Types for function values and parameters within this FunctionSet
// TODO for now I am using std::string name to "mock" a type
typedef std::string FunctionType;

// Describes one function in the set.
//     TODO mocked with strings
//     TODO inside FunctionSet or global?
//     TODO added support for "ephemeral constant". Not sure if it belongs here.
class FunctionDescription
{
public:
    FunctionDescription(){}
    FunctionDescription(const std::string& name_,
                        const std::string& return_type_,
                        const std::vector<std::string>& parameter_types_)
      : FunctionDescription(name_, return_type_, parameter_types_, nullptr) {}
    FunctionDescription(const std::string& name_,
                        const std::string& return_type_,
                        const std::vector<std::string>& parameter_types_,
                        std::function<std::string()> ephemeral_generator_)
      : name(name_),
        return_type(return_type_),
        parameter_types(parameter_types_),
        ephemeral_generator(ephemeral_generator_) {}
    std::string name;
    std::string return_type;
    std::vector<std::string> parameter_types;
    std::function<std::string()> ephemeral_generator;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO Aug 18: to "reduce confusion" I will define two NEW types:
//     GpType
//     GpFunction
// and start on new constructors for FunctionSet that accepts collections of
// these two, then does all caching so that subsequent lookups are fast.
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
    // TODO minSizeToTerminateType
    int min_size_to_terminate_ = std::numeric_limits<int>::max();
};

// TODO This has to deal somehow with types themsleves (as pointers?) AND
//      as std::string type names.
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

    // minSizeToTerminateFunction
};

// Down here because it needs both GpType and GpFunction to be defined.
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
//        std::cout << ")";
    }
    std::cout << "." << std::endl;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class FunctionSet
{
public:
    FunctionSet(){}
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
            // TODO (Aug 18) old style, I expect this to become obsolete soon:
            if (eg) addFunction(gp_type.name(), gp_type.name(), {}, eg);
        }
        // Process each of the GpFunction specifications (copy then modify)
        for (GpFunction func : function_specs)
        {
            // Character string name of this function's return type.
            std::string rtn = func.returnTypeName();
            // TODO (Aug 18) old style, I expect this to become obsolete soon:
            addFunction(func.name(), rtn, func.parameterTypeNames());
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
                std::cout << "NOT recursive: " << func.returnTypeName();
                GpType& rt = *lookupGpTypeByName(rtn);
                // TODO this is "min size to terminate type" ASSUMING all
                // parameters are ephemeral constants. Make more general.
                int mstt = 1 + int(func.parameterTypeNames().size());
                if (mstt < rt.minSizeToTerminate())
                    rt.setMinSizeToTerminate(mstt);
                std::cout << " min size to terminate: " << mstt << std::endl;
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
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void addType(const std::string& name)
    {
        int id = int(types_.size());
        types_[name] = id;
    }
    
    int typeIdFromName(const std::string& name)
    {
        assert("unknown type" && (types_.find(name) != types_.end()));
        return types_[name];
    }
        
    void addFunction(const std::string& name,
                     const std::string& return_type,
                     const std::vector<std::string>& parameter_types,
                     std::function<std::string()> ephemeral_generator)
    {
        addFunction({name, return_type, parameter_types, ephemeral_generator});
    }
    void addFunction(const std::string& name,
                     const std::string& return_type,
                     const std::vector<std::string>& parameter_types)
    {
        addFunction({name, return_type, parameter_types});
    }
    void addFunction(const FunctionDescription& fd)
    {
        functions_[fd.name] = fd;
    }
    
    // Find collection of functions in this set that return the given type.
    // TODO very prototype, we can cache this information per-type
    // TODO does brute force search, stores results in "results"
    void findAllFunctionReturningType(const std::string& return_type,
                                      std::vector<std::string>& results)
    {
        results.clear();
        for (auto& pair : functions_)
        {
            FunctionDescription& fd = pair.second;
            if (return_type == fd.return_type) results.push_back(fd.name);
        }
        assert("no function of given return_type found" && !results.empty());
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
//    // Given a type, lookup the min size to terminate.
//    // TODO this is all hard coded and needs a real implementation
//    int minSizeToTerminateType(const FunctionType& function_type)
//    {
//        // for first pass, lets assume we know type "Float *" needs size 1
//        // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
//        // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
//        // we want to derive these at FunctionSet definition time.
//        std::map<std::string, int> type_to_min_size;
//        type_to_min_size["Float_01"] = 1;
//        type_to_min_size["Float_02"] = 1;
//        type_to_min_size["Float_0_10"] = 1;
//        type_to_min_size["Float_m5p5"] = 1;
//        type_to_min_size["Vec2"] = 3;
//        type_to_min_size["Texture"] = 4;
//        assert("Unknown function type" &&
//               type_to_min_size.find(function_type) != type_to_min_size.end());
//        return type_to_min_size[function_type];
//    }
    
//    // Given a type, lookup the min size to terminate.
//    // TODO this is all hard coded and needs a real implementation
//    // TODO now superceeded by GpType::minSizeToTerminate()
//    int minSizeToTerminateType(const FunctionType& function_type)
//    {
//        // for first pass, lets assume we know type "Float *" needs size 1
//        // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
//        // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
//        // we want to derive these at FunctionSet definition time.
//        std::map<std::string, int> type_to_min_size;
//        type_to_min_size["Float_01"] = 1;
//        type_to_min_size["Float_02"] = 1;
//        type_to_min_size["Float_0_10"] = 1;
//        type_to_min_size["Float_m5p5"] = 1;
//        type_to_min_size["Vec2"] = 3;
//        type_to_min_size["Texture"] = 4;
//        assert("Unknown function type" &&
//               type_to_min_size.find(function_type) != type_to_min_size.end());
//
//        assert("new/old minSizeToTerminate mismatch" &&
//               (type_to_min_size[function_type] !=
//                name_to_gp_type_[function_type].minSizeToTerminate()));
//
//        return type_to_min_size[function_type];
//    }

    // Given a type, lookup the min size to terminate.
    // TODO this is all hard coded and needs a real implementation
    // TODO now superceeded by GpType::minSizeToTerminate()
    int minSizeToTerminateType(const FunctionType& function_type)
    {
//        // for first pass, lets assume we know type "Float *" needs size 1
//        // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
//        // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
//        // we want to derive these at FunctionSet definition time.
//        std::map<std::string, int> type_to_min_size;
//        type_to_min_size["Float_01"] = 1;
//        type_to_min_size["Float_02"] = 1;
//        type_to_min_size["Float_0_10"] = 1;
//        type_to_min_size["Float_m5p5"] = 1;
//        type_to_min_size["Vec2"] = 3;
//        type_to_min_size["Texture"] = 4;
//        assert("Unknown function type" &&
//               type_to_min_size.find(function_type) != type_to_min_size.end());
//
//        //    debugPrint(function_type);
//        //    debugPrint(type_to_min_size[function_type]);
//        //    debugPrint(name_to_gp_type_[function_type].minSizeToTerminate());
//        assert("new/old minSizeToTerminate mismatch" &&
//               (type_to_min_size[function_type] ==
//                name_to_gp_type_[function_type].minSizeToTerminate()));
//
//        return type_to_min_size[function_type];
        
        return lookupGpTypeByName(function_type)->minSizeToTerminate();
    }



//        // Given a type, lookup the minimum "size" to terminate a subtree.
//        // TODO this is all hard coded and needs a real implementation
//        int minSizeToTerminateType(const FunctionType& function_type)
//        {
//            // for first pass, lets assume we know type "Float *" needs size 1
//            // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
//            // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
//            // we want to derive these at FunctionSet definition time.
//            std::map<std::string, int> type_to_min_size;
//            type_to_min_size["Float_01"] = 1;
//            type_to_min_size["Float_02"] = 1;
//            type_to_min_size["Float_0_10"] = 1;
//            type_to_min_size["Float_m5p5"] = 1;
//            type_to_min_size["Vec2"] = 3;
//            type_to_min_size["Texture"] = 4;
//    //        assert("Unknown function type" &&
//    //               type_to_min_size.find(function_type) != type_to_min_size.end());
//    //        return type_to_min_size[function_type];
//            auto found = type_to_min_size.find(function_type);
//            assert("Unknown function type" && found != type_to_min_size.end());
//            return found->second;
//        }
    
    // TODO temporary prototype to find all FunctionTypes in this FunctionSet
    //      as a set of strings
    void fillInSetOfTypes(std::set<FunctionType>& set_of_function_types)
    {
        for (auto& pair : functions_)
        {
            FunctionDescription& fd = pair.second;
            set_of_function_types.insert(fd.return_type);
            for (auto& parameter_type : fd.parameter_types)
            {
                set_of_function_types.insert(parameter_type);
            }
        }
    }
    
//        // Given a type, lookup the minimum "size" to terminate a subtree.
//        // TODO this is all hard coded and needs a real implementation
//        int minSizeToTerminateType(const FunctionType& function_type)
//        {
//            // for first pass, lets assume we know type "Float *" needs size 1
//            // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
//            // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
//            // we want to derive these at FunctionSet definition time.
//            std::map<std::string, int> type_to_min_size;
//
//            //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//    //        type_to_min_size["Float_01"] = 1;
//    //        type_to_min_size["Float_02"] = 1;
//    //        type_to_min_size["Float_0_10"] = 1;
//    //        type_to_min_size["Float_m5p5"] = 1;
//
//            type_to_min_size["Vec2"] = 3;
//            type_to_min_size["Texture"] = 4;
//
//
//            // For each FunctionType
//            std::set<FunctionType> set_of_function_types;
//            fillInSetOfTypes(set_of_function_types);
//            for (auto& type : set_of_function_types)
//            {
//                std::vector<std::string> functions;
//                findAllFunctionReturningType(type, functions);
//                // For each function of this type
//                for (auto& function_name : functions)
//                {
//                    FunctionDescription fd = functions_[function_name];
//                    if (fd.ephemeral_generator) type_to_min_size[type] = 1;
//
//                    // TODO more stuff here!!!! TODO TODO TODO TODO TODO TODO TODO
//
//    //                minSizeToTerminateFunction ... uh oh, this is co-recursive!!!
//                }
//            }
//            //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
//            // Lookup given FunctionType in table, return if found, else error.
//            auto found = type_to_min_size.find(function_type);
//            assert("Unknown function type" && found != type_to_min_size.end());
//            return found->second;
//        }

    // TODO QQQ
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // TODO new experimental thing that estimates how much "size" is required to
    // "terminate" a given FunctionDescription fd
    int minSizeToTerminateFunction(const std::string& function_name)
    {
        assert("Unknown function name" &&
               functions_.find(function_name) != functions_.end());
        FunctionDescription fd = functions_[function_name];
        return minSizeToTerminateFunction(fd);
    }
    int minSizeToTerminateFunction(const FunctionDescription& fd)
    {
        // We need 1 for the function name itself (or an ephemeral constant).
        int size = 1;
        // Then loop over all parameter types.
        for (auto& parameter_type : fd.parameter_types)
        {
            size += minSizeToTerminateType(parameter_type);
        }
        return size;
    }
    
    // Randomly select a function in this set that return the given type and can
    // be implemented in no more than max_size.
    std::string randomFunctionOfTypeInSize(int max_size,
                                           const FunctionType& return_type)
    {
        std::vector<std::string> functions_returning_type;
        findAllFunctionReturningType(return_type, functions_returning_type);
        std::vector<std::string> fit_size;
        for (auto& function_name : functions_returning_type)
        {
            if (max_size >= minSizeToTerminateFunction(function_name))
            {
                fit_size.push_back(function_name);
            }
        }
        if (fit_size.empty() && dp)
        {
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type);
        }
        assert(!fit_size.empty());
        return fit_size.at(rs_.nextInt() % fit_size.size());
    }
    
    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // nodes (function calls or constants) in the resulting program. The program
    // computes a value of "return_type".
    void makeRandomProgram(int max_size,
                           const FunctionType& return_type,
                           int& output_actual_size,
                           std::string& source_code)
    {
        if (dp)
        {
            dp_prefix();
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type);
            dp_prefix(); debugPrint(source_code);
            dp_prefix(); std::cout << std::endl;
        }
        makeRandomProgramRoot(max_size,
                              return_type,
                              randomFunctionOfTypeInSize(max_size, return_type),
                              output_actual_size,
                              source_code);
        if (dp)
        {
            dp_prefix();
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        }
    }

    void makeRandomProgramRoot(int max_size,
                               const FunctionType& return_type,
                               const std::string& root_function_name,
                               int& output_actual_size,
                               std::string& source_code)
    {
        FunctionDescription fd = functions_[root_function_name];
        if (dp)
        {
            dp_prefix(); debugPrint(max_size);
            dp_prefix(); debugPrint(return_type);
            dp_prefix(); debugPrint(root_function_name);
            dp_prefix(); debugPrint(fd.parameter_types.size());
        }
        output_actual_size++;  // for "function_name" (or epheneral) itself
        // "Epheneral constant" or normal function.
        if (fd.ephemeral_generator)
        {
            source_code += fd.ephemeral_generator();  // TODO log
        }
        else
        {
            int size_used = 0;
            int count = int(fd.parameter_types.size());
            source_code += root_function_name + "(";  // TODO log
            bool first = true;   // TODO log
            // Loop over each parameter of "function_name" generating a subtree.
            for (auto& parameter_type : fd.parameter_types)
            {
                if (dp)
                {
                    dp_prefix(); std::cout << "((((((((((((((((((";
                    std::cout << " param=" << fd.parameter_types.size() - count;
                    std::cout << " type=" << parameter_type;
                    std::cout << std::endl;
                }
                if (first) first = false; else source_code += ", ";  // TODO log
                int fair_share = (max_size - (1 + size_used)) / count;
                int min_size_for_type = minSizeToTerminateType(parameter_type);
                int subtree_max_size = std::max(fair_share, min_size_for_type);
                if (dp) { dp_prefix(); debugPrint(subtree_max_size); }
                int subtree_actual_size = 0;
                std::string subtree_source;
                dp_depth++;
                makeRandomProgram(subtree_max_size, parameter_type,
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
    }

    void printSet()
    {
        for (auto& pair : types_)
        {
            std::cout << "type: " << pair.first << " " << pair.second;
            std::cout << std::endl;
        }
        for (auto& pair : functions_)
        {
            bool first = true;
            FunctionDescription& fd = pair.second;
            std::cout << "function: " << fd.name << ", returns: ";
            std::cout << fd.return_type << ", parameters: (";
            for (auto& pt : fd.parameter_types)
            {
                if (first) first = false; else std::cout << ", ";
                std::cout << pt;
            }
            std::cout << ")" << std::endl;
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO for reference:
        // std::map<std::string, GpType> name_to_gp_type_;
        // std::map<std::string, GpFunction> name_to_gp_function_;
        for (auto& pair : nameToGpTypeMap()) pair.second.print();
        for (auto& pair : nameToGpFunctionMap()) pair.second.print();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    
    // Accessor for RandomSequence, perhaps only needed for testing?
    RandomSequence& rs() { return rs_; }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


private:
    RandomSequence rs_;
    // TODO these are "mocks" to allow getting started.
    // just to get started, assume types are strings with int ID.
    std::map<std::string, int> types_;
    std::map<std::string, FunctionDescription> functions_;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO Aug 18: to "reduce confusion" I will define two NEW types:
    //     GpType
    //     GpFunction
    
    // TODO these should be covered by accessors.
    std::map<std::string, GpType> name_to_gp_type_;
    std::map<std::string, GpFunction> name_to_gp_function_;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
