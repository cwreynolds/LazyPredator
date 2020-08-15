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
                     const std::vector<std::string>& parameter_types,
                     std::function<void()> ephemeral_generator)
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
    
    // Randomly select a function in this set that returns the given type AND is
    // terminal. Here terminal will mean none of it parameters are of the given
    // type.
    //
    // TODO Ideally it should be one producing the smallest subtree.
    //
    // TODO very prototype, we can probably cache this information per-type
    // TODO should not call rand() directly
    // TODO returning a string just for prototyping
    std::string terminalFunctionReturningType(const std::string& return_type)
    {
        std::vector<std::string> functions_returning_type;
        findAllFunctionReturningType(return_type, functions_returning_type);
        std::vector<std::string> terminals;
        for (auto& function_name : functions_returning_type)
        {
            bool is_terminal = true;
            FunctionDescription fd = functions_[function_name];
            for (auto& pt : fd.parameter_types)
                if (pt == return_type) is_terminal = false;
            if (is_terminal) { terminals.push_back(function_name); }
        }
        return terminals.at(rs_.nextInt() % terminals.size());
    }
    
    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // function calls in the resulting program. The program computes a value of
    // "return_type".
    // TODO just for prototype, randomFunctionReturningType should return fd
    void makeRandomProgram(int max_size, const FunctionType& return_type)
    {
        int ignore_actual_size = 0;
        makeRandomProgram(max_size, return_type, ignore_actual_size);
    }
    // version to keep track of actual size (maybe this version is private?)
    void makeRandomProgram(int max_size,
                           const FunctionType& return_type,
                           int& output_actual_size)
    {
        // if max_size too small: must choose terminal (Uniform/ColorNoise)
        int min_size = 5;
        std::string function_name = (max_size < min_size ?
                                     terminalFunctionReturningType(return_type) :
                                     randomFunctionReturningType(return_type));
        FunctionDescription fd = functions_[function_name];
        output_actual_size++;
        // "Epheneral constant" or normal function.
        if (fd.ephemeral_generator)
        {
            fd.ephemeral_generator();
        }
        else
        {
            int size_used = 0;
            int count = int(fd.parameter_types.size());
            std::cout << function_name << "(";  // TODO log
            bool first = true;   // TODO log
            for (auto& pt : fd.parameter_types)
            {
                if (first) first = false; else std::cout << ", ";  // TODO log
                // TODO numerator = 1 for fd + 1 of each Texture operator
                int subtree_max_size = ((max_size - (size_used - count)) /
                                        (count == 0 ? 1 : count));
                int subtree_actual_size = 0;
                makeRandomProgram(subtree_max_size, pt, subtree_actual_size);
                output_actual_size += subtree_actual_size;
                size_used += subtree_actual_size;
                count--;
            }
            std::cout << ")";  // TODO log
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
