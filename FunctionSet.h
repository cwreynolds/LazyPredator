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

// Describes one function in the set.
//     TODO mocked with strings
//     TODO inside FunctionSet or global?
class FunctionDescription
{
public:
    FunctionDescription(){}
    FunctionDescription(const std::string& name_,
                        const std::string& return_type_,
                        const std::vector<std::string>& parameter_types_)
      : name(name_),
        return_type(return_type_),
        parameter_types(parameter_types_) {}
    std::string name;
    std::string return_type;
    std::vector<std::string> parameter_types;
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
        return results.at(rand() % results.size()); // TODO!!!
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

private:
    // TODO these are "mocks" to allow getting started.
    // just to get started, assume types are strings with int ID.
    std::map<std::string, int> types_;
    std::map<std::string, FunctionDescription> functions_;
};
