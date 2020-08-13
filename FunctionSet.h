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
                        std::function<void()> ephemeral_generator_)
      : name(name_),
        return_type(return_type_),
        parameter_types(parameter_types_),
        ephemeral_generator(ephemeral_generator_) {}
    std::string name;
    std::string return_type;
    std::vector<std::string> parameter_types;
    std::function<void()> ephemeral_generator;
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
    }
    
    // Randomly select a function in this set that return the given type.
    // TODO very prototype, we can probably cache this information per-type
    // TODO should not call rand() directly
    // TODO returning a string just for prototyping
    std::string randomFunctionReturningType(const std::string& return_type)
    {
        std::vector<std::string> results;
        findAllFunctionReturningType(return_type, results);
        return results.at(rs_.nextInt() % results.size());
    }
    
    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // function calls in the resulting program. The program computes a value of
    // "return_type".
    void makeRandomProgram(int max_size, const FunctionType& return_type)
    {
        // debugPrint(max_size);
        
        // TODO just for prototype, randomFunctionReturningType should return fd
        std::string function_name = randomFunctionReturningType(return_type);
        FunctionDescription fd = functions_[function_name];
        
        auto ephemeral = fd.ephemeral_generator;
        
        if (ephemeral)
            ephemeral();
        else
            std::cout << function_name << "(";  // TODO log
        
        // TODO this "knows" that only parameters of the main return_type can be
        // the root of subtrees. But that is not necessarily the case. Needs to
        // be better.
        int count = 0;  // count parameters of return_type
        for (auto& pt : fd.parameter_types)
        {
            if (pt == return_type) count++;
        }
//        int subtree_max_size = (max_size - 1) / (count == 0 ? 1 : count);
        // TODO numerator = 1 for fd + 1 of each Texture operator
        int subtree_max_size = ((max_size - count) /
                                (count == 0 ? 1 : count));

        bool first = true;   // TODO log
        for (auto& pt : fd.parameter_types)
        {
            if (!ephemeral)
                { if (first) first = false; else std::cout << ", "; }  // TODO log
            makeRandomProgram(subtree_max_size, pt);
        }

        if (!ephemeral) std::cout << ")";  // TODO log
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
