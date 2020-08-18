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

class FunctionSet
{
public:
    FunctionSet(){}
        
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
    // TODO very prototype, we can probably cache this information per-type
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

    // Given a type, lookup the min size to terminate.
    // TODO this is all hard coded and needs a real implementation
    int minSizeToTerminateType(const FunctionType& function_type)
    {
        // for first pass, lets assume we know type "Float *" needs size 1
        // (ephemeral constant), type "Vec2" requires size 3 (Vec2(x, y)),
        // and type "Texture" requires size 4: Uniform(r, g, b). Eventually
        // we want to derive these at FunctionSet definition time.
        std::map<std::string, int> type_to_min_size;
        type_to_min_size["Float_01"] = 1;
        type_to_min_size["Float_02"] = 1;
        type_to_min_size["Float_0_10"] = 1;
        type_to_min_size["Float_m5p5"] = 1;
        type_to_min_size["Vec2"] = 3;
        type_to_min_size["Texture"] = 4;
        assert("Unknown function type" &&
               type_to_min_size.find(function_type) != type_to_min_size.end());
        return type_to_min_size[function_type];
    }
    
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
    }
    
    // Accessor for RandomSequence, perhaps only needed for testing?
    RandomSequence& rs() { return rs_; }

private:
    RandomSequence rs_;
    // TODO these are "mocks" to allow getting started.
    // just to get started, assume types are strings with int ID.
    std::map<std::string, int> types_;
    std::map<std::string, FunctionDescription> functions_;
};
