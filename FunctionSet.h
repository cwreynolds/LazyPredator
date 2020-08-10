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

class FunctionSet
{
public:
    // TODO default constructor -- just for testing
    FunctionSet()
    {
        
    }
    
//    void addType(const std::string& name)
//    {
//        debugPrint(types_.size());
//
//        int id = int(types_.size());
//        types_[name] = id;
//
//        for (auto& pair : types_)
//        {
//            std::cout << "{" << pair.first << ", " << pair.second << "}";
//            std::cout << std::endl;
//        }
//    }
    
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
                     const std::vector<std::string>& parameter_types)
    {
        functions_[name] = parameter_types;
        
        for (auto& pair : functions_)
        {
            bool first = true;
            std::cout << "{" << pair.first << ", {";
            for (auto& s : pair.second)
            {
                if (first) first = false; else std::cout << ", ";
                std::cout << s;
            }
            std::cout << "}}" << std::endl;
        }
    }
    
private:
    // just to get started, assume types are strings with int ID.
    std::map<std::string, int> types_;
    std::map<std::string, std::vector<std::string>> functions_;
};
