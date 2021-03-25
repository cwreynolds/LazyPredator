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
#include "Utilities.h"
#include "GpType.h"
#include "GpFunction.h"
#include "GpTree.h"

// Used only below in FunctionSet, then undef-ed at end of file.
#define name_lookup_util(name, map)               \
[&]()                                             \
{                                                 \
    auto it = map.find(name);                     \
    assert("unknown type" && (it != map.end()));  \
    return &(it->second);                         \
}()

// Defines the function set used in a STGP run. Consists of a collection of
// GpTypes and one of GpFunctions. Supports creation of random program drawn
// from those types and functions, given a max_size and root type.
// TODO must support crossover and mutation.
class FunctionSet
{
public:
    FunctionSet(){}
    // New constructor using vectors of GpType and GpFunction.
    // TODO eventually this needs to be rewritten to be smaller.
    // Allows crossover_min_size to default to 1.
    FunctionSet(const std::vector<GpType>& type_specs,
                const std::vector<GpFunction>& function_specs)
        : FunctionSet(type_specs, function_specs, 1) {}
    // Version with collection of GpTypes, GpFunctions, and crossover_min_size.
    FunctionSet(const std::vector<GpType>& type_specs,
                const std::vector<GpFunction>& function_specs,
                int crossover_min_size)
        : crossover_min_size_(crossover_min_size)
    {
        // Process each GpType specifications, make local copy to modify.
        for (GpType gp_type : type_specs)
        {
            bool has_eg = gp_type.hasEphemeralGenerator();
            if (has_eg) gp_type.setMinSizeToTerminate(1);
            // Insert updated GpType into by-name map. (Copied again into map.)
            addGpType(gp_type);
            // Root type (for trees of this FS) defaults to first GpType listed.
            if (!getRootType()) setRootType(lookupGpTypeByName(gp_type.name()));
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
            gp_function.linkToReturnType();
        }
        // Set minSizeToTerminate() for each GpFunction.
        for (auto& [name, gp_function] : nameToGpFunctionMap())
        {
            int min_size = minSizeToTerminateFunction(gp_function);
            gp_function.setMinSizeToTerminate(min_size);
        }
    }
    
    // What is the minimum "size" required to terminate a program subtree with
    // the given function at the root? At FunctionSet construction time, this
    // computes the number which is then stored on each GpFunction instance.
    int minSizeToTerminateFunction(const GpFunction& gp_function) const
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
                                           const GpType& return_type) const
    {
        std::vector<GpFunction*> ok;
        for (auto& gp_function : return_type.functionsReturningThisType())
        {
            int ms = gp_function->minSizeToTerminate();
            if (max_size >= ms) { ok.push_back(gp_function); }
        }
        // Intended to be used only for testing and debugging
        if (function_filter) { function_filter(ok); };
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        return (ok.empty() ? nullptr : ok.at(LPRS().randomN(ok.size())));
        
//        return (ok.empty() ? nullptr : GpFunction::weightedRandomSelect(ok));
        
        return (ok.empty() ? nullptr : weightedRandomSelect(ok));

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Return a GpFunction* randomly selected from a given collection of them.
    // Selection is made according to each GpFunction's selectionWeight().
    GpFunction* weightedRandomSelect(const std::vector<GpFunction*>&
                                     gp_functions) const
    {
        GpFunction* result = nullptr;
        float total_weight = 0;
        for (auto f : gp_functions) { total_weight += f->selectionWeight(); }
        float random = LPRS().random2(0.0f, total_weight); // TODO which RS?
        float previous = 0;
        for (auto gp_function : gp_functions)
        {
            float next = previous + gp_function->selectionWeight();
            if (random < next) { result = gp_function; break; }
            previous = next;
        }
        return result;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Creates a random program (nested expression) using the "language" defined
    // in this FunctionSet. Parameter "max_size" is upper bound on the number of
    // nodes (function calls or constants) in the resulting program. The program
    // returns a value of "return_type" from its root.
    void makeRandomTree(int max_size,
                        const GpType& return_type,
                        int& output_actual_size,
                        GpTree& gp_tree) const
    {
        // Find all function whose value is return_type, for which a subtree can
        // be constructed in max_size or fewer nodes, and select on randomly.
        GpFunction* rf = randomFunctionOfTypeInSize(max_size, return_type);
        if (rf)
        {
            // If found, recurse on a subtree with that function at the root.
            makeRandomTreeRoot(max_size, return_type, *rf,
                               output_actual_size, gp_tree);
        }
        else if (return_type.hasEphemeralGenerator())
        {
            // If no function found, but this type has an ephemeral generator,
            // use it to generate a leaf constant, ending recursion.
            output_actual_size++;
            // TODO should pass rs() into the generator for repeatability.
            std::any leaf_value = return_type.generateEphemeralConstant();
            gp_tree.setRootValue(leaf_value, return_type);
        }
        else
        {
            // TODO better way to signal this FunctionSet specification error?
            std::cout << "No function or epheneral constant found for GpType ";
            std::cout << return_type.name() << " remaining size: " << max_size;
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Overload to allow passing return_type_name as a string from top level.
    void makeRandomTree(int max_size,
                        const std::string& return_type_name,
                        int& output_actual_size,
                        GpTree& gp_tree) const
    {
        makeRandomTree(max_size,
                       *lookupGpTypeByName(return_type_name),
                       output_actual_size,
                       gp_tree);
    }

    // Overload to allow passing return_type_name as a string from top level,
    // and NOT passing in output_actual_size.
    void makeRandomTree(int max_size,
                        const std::string& return_type_name,
                        GpTree& gp_tree) const
    {
        int output_actual_size = 0;
        makeRandomTree(max_size, return_type_name, output_actual_size, gp_tree);
    }

    // Overload to pass ONLY "max_size" and "output_gp_tree" from top level.
    // "return_type" is assumed to be first GpType listed in the FunctionSet.
    // "output_actual_size" is provided locally and passed in.
    void makeRandomTree(int max_size, GpTree& output_gp_tree) const
    {
        int output_actual_size = 0;
        makeRandomTree(max_size,
                       *getRootType(),
                       output_actual_size,
                       output_gp_tree);
    }

    void makeRandomTreeRoot(int max_size,
                            const GpType& return_type,
                            const GpFunction& root_function,
                            int& output_actual_size,
                            GpTree& gp_tree) const
    {
        output_actual_size++;  // for "function_name" (or epheneral) itself
        int size_used = 0;
        int count = int(root_function.parameterTypes().size());
        // Set root function in given GpTree object
        gp_tree.setRootFunction(root_function);
        // For each parameter of root, add/allocate a subtree in the GpTree.
        // Important this happen first so no iterators are invalidated later.
        gp_tree.addSubtrees(root_function.parameterTypes().size());
        // TODO do this in a cleaner way after it is working
        int subtree_index = 0;
        // Loop over each parameter of "function_name" generating a subtree.
        for (auto& parameter_type : root_function.parameterTypes())
        {
            int fair_share = (max_size - (1 + size_used)) / count;
            int min_size_for_type = parameter_type->minSizeToTerminate();
            int subtree_max_size = std::max(fair_share, min_size_for_type);
            int subtree_actual_size = 0;
            std::string subtree_source;
            makeRandomTree(subtree_max_size,
                           *parameter_type,
                           subtree_actual_size,
                           gp_tree.subtrees().at(subtree_index));
            // TODO do this in a cleaner way after it is working
            subtree_index++;
            output_actual_size += subtree_actual_size;
            size_used += subtree_actual_size;
            count--;
        }
    }

    void print() const
    {
        std::cout << std::endl;
        std::cout << nameToGpTypeMap().size() << " GpTypes: " << std::endl;
        for (auto& [n, t] : nameToGpTypeMap()) t.print();
        std::cout << std::endl;
        std::cout << nameToGpFunctionMap().size() << " GpFunctions: ";
        std::cout << std::endl;
        for (auto& [n, f] : nameToGpFunctionMap()) f.print();
        std::cout << std::endl;
    }

    // Map string names to (pointers to) GpTypes/GpFunction objects, for both
    // const (public, read only) and non-const (private, writable, for internal
    // use only during constructor). Use macro to remove code duplication.
    const GpType* lookupGpTypeByName(const std::string& name) const
        { return name_lookup_util(name, nameToGpTypeMap()); }
    const GpFunction* lookupGpFunctionByName(const std::string& name) const
        { return name_lookup_util(name, nameToGpFunctionMap()); }
    
    // Add new GpType/GpFunction to FunctionSet, stored in a name-to-object map.
    void addGpType(GpType& type) { name_to_gp_type_[type.name()] = type; }
    void addGpFunction(GpFunction& f) { name_to_gp_function_[f.name()] = f; }
    // Get reference to name-to-object maps of all GpType/GpFunction objects
    std::map<std::string, GpType>& nameToGpTypeMap()
        { return name_to_gp_type_; }
    const std::map<std::string, GpType>& nameToGpTypeMap() const
        { return name_to_gp_type_; }
    std::map<std::string, GpFunction>& nameToGpFunctionMap()
        { return name_to_gp_function_; }
    const std::map<std::string, GpFunction>& nameToGpFunctionMap() const
        { return name_to_gp_function_; }
    
    // The type returned from the root of trees built from this function set.
    const GpType*  getRootType() const { return root_type_; }
    void setRootType(GpType* gp_type) { root_type_ = gp_type; }
    
    // Given a mutable collection of functions (std::vector<GpFunction*>) modify
    // it in place. (TODO maybe better to return a modified copy, by value?)
    // Intended to be used only for testing and debugging by removing candidate
    // GpFunctions from consideration when generating random GpTrees.
    static inline std::function<void(std::vector<GpFunction*>&)>
        function_filter = nullptr;

    // The smallest size for a subtree (GpTree) to be exchanged between parent
    // GpTrees during crossover. The default of 1 allows all subtrees including
    // leaf values such as numeric constants. Values of 2 or more exclude those.
    int getCrossoverMinSize() const { return crossover_min_size_; }
    
private:
    // These maps are used both to store the GpType and GpFunction objects,
    // plus to look up those objects from their character string names.
    std::map<std::string, GpType> name_to_gp_type_;
    std::map<std::string, GpFunction> name_to_gp_function_;
    // Non-const versions for use only in constructor.
    GpType* lookupGpTypeByName(const std::string& name)
        { return name_lookup_util(name, nameToGpTypeMap()); }
    GpFunction* lookupGpFunctionByName(const std::string& name)
        { return name_lookup_util(name, nameToGpFunctionMap()); }
    // The type returned from the root of trees built from this function set.
    GpType* root_type_ = nullptr;
    // The smallest size for a subtree (GpTree) to be exchanged between parent
    // GpTrees during crossover. The default of 1 allows all subtrees including
    // leaf values such as numeric constants. Values of 2 or more exclude those.
    int crossover_min_size_ = 1;
};

#undef name_lookup_util
