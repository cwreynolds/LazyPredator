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
#include <any>
#include <set>
#include <limits>
#include "Utilities.h"

// TODO -- LPRS() temporary global RandomSequence used for both makeRandomTree
//         and ephemeral generators. Move somewhere else? Redesign?
class LP
{
public:
    static RandomSequence& randomSequence() { return random_sequence; }
private:
    static inline RandomSequence random_sequence;
};
inline RandomSequence& LPRS() { return LP::randomSequence(); }

class GpTree;      // Forward reference to class defined below.
class GpFunction;  // Forward reference to class defined below.

// Class to represent types in "strongly typed genetic programming".
class GpType
{
public:
    // Default constructor.
    GpType(){}
    // Constructor for name only.
    GpType(const std::string& name) : name_(name) {}
    // Full constructor, specify all parameters.
    GpType(const std::string& name,
           std::function<std::any()> ephemeral_generator,
           std::function<std::string(std::any a)> to_string,
           std::function<std::any(std::any)> jiggle)
      : name_(name),
        ephemeral_generator_(ephemeral_generator),
        to_string_(to_string),
        jiggle_(jiggle) {}
    // Constructor for ranged numeric types (name, range_min, range_max).
    template <typename T>
    GpType(const std::string& name, T range_min, T range_max)
      : GpType(name, range_min, range_max, defaultJiggleScale()) {}
    // Constructor for ranged numeric types, plus custom jiggle scale.
    template <typename T>
    GpType(const std::string& name, T range_min, T range_max, float jiggle_scale)
      : GpType(name,
               [=](){ return std::any(LPRS().random2(range_min, range_max)); },
               any_to_string<T>,
               [=](std::any x) { return jiggle(std::any_cast<T>(x), range_min,
                                               range_max, jiggle_scale); }){}
    // Accessor for name.
    const std::string& name() const { return name_; }
    // Does this type have an ephemeral generator?
    bool hasEphemeralGenerator() const { return bool(ephemeral_generator_); }
    // Generate an ephemeral constant.
    std::any generateEphemeralConstant() const
    {
        assert(hasEphemeralGenerator());
        return ephemeral_generator_();
    }
    // Does this type have a jiggle function?
    bool hasJiggler() const { return bool(jiggle_); }
    // Generate an ephemeral constant.
    std::any jiggleConstant(std::any current_value) const
    {
        assert(hasJiggler());
        return jiggle_(current_value);
    }
    // Access collection of (pointers to) GpFunction that return this type.
    const std::vector<GpFunction*>& functionsReturningThisType() const
        { return functions_returning_this_type_; }
    // Add a GpFunction to the collection of those that return this type.
    void addFunctionReturningThisType(GpFunction* gp_function_pointer)
        { functions_returning_this_type_.push_back(gp_function_pointer); }
    // Minimum "size" of tree returning this type from root;
    int minSizeToTerminate() const { return min_size_to_terminate_; }
    void setMinSizeToTerminate(int s) { min_size_to_terminate_ = s; }
    void print() const;
    bool valid() const
    {
        return ((!name().empty()) &&
                // TODO may be wrong given later changes to ephemeral constants:
                (!functionsReturningThisType().empty()) &&
                (minSizeToTerminate() < std::numeric_limits<int>::max()));
    }
    // Uses function (supplied in constructor) to make a string of std::any
    // value via this GpType's concrete c++ type.
    std::string to_string(std::any a) const { return to_string_(a); }
    // Default max jiggle: a scale factor for magnitude of noise added to a
    // ranged numeric GpType by "jiggle mutation." Zero-centered noise added is
    // uniformly distributed on the interval [-m, +m] where m is: ((range_max -
    // range_min) * jiggle_factor). The interval [-m, +m] is then "pre-clipped"
    // to the range of the GpType.
    static float defaultJiggleScale() { return 0.05; }
    // Utility template function for jiggle mutation handlers.
    template <typename T> static T jiggle(T x, T min, T max, float jiggle_scale)
    {
        T max_jiggle = (max - min) * jiggle_scale;
        return LPRS().random2(std::max(min, x - max_jiggle),
                              std::min(max, x + max_jiggle));
    }
private:
    std::string name_;
    // Function to generate an ephemeral constant.
    std::function<std::any()> ephemeral_generator_ = nullptr;
    // Function to generate string representation of a value of this GpType.
    std::function<std::string(std::any a)> to_string_ = nullptr;
    // Function to jiggle/jitter an ephemeral constant.
    std::function<std::any(std::any)> jiggle_ = nullptr;
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
               const std::vector<std::string>& parameter_type_names,
               std::function<std::any(const GpTree& t)> eval)
      : name_(name),
        return_type_name_(return_type_name),
        parameter_type_names_(parameter_type_names),
        eval_(eval) {}
    // String name of this GpFunction.
    const std::string& name() const { return name_; }
    // String name of this GpFunction's return type.
    const std::string& returnTypeName() const { return return_type_name_; }
    // Pointer to this GpFunction's return GpType.
    GpType* returnType() const { return return_type_; }
    // GpTypes for all parameters of this GpFunction.
    const std::vector<GpType*>& parameterTypes() const
        { return parameter_types_; }
    // String names for GpTypes of all parameters of this GpFunction.
    const std::vector<std::string>& parameterTypeNames() const
        { return parameter_type_names_; }
    // Set GpTypes for return type and all parameter types of this GpFunction.
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
    // Evaluate (execute) a GpTree with this function at root
    // TODO probably should assert they match (this and tree root GpFunction)
    std::any eval(const GpTree& tree) const { return eval_(tree);  }
    // Print description of this GpFunction to std::cout.
    void print() const
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
    std::function<std::any(const GpTree& t)> eval_ = nullptr;
};

// Down here because it requires both GpType and GpFunction to be defined.
inline void GpType::print() const
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
    void setFunction(const GpFunction& function)
    {
        root_function_ = &function;
        root_type_ = function.returnType();
    }
    // Get GpType returned by the root of this GpTree.
    const GpType& getType() const { return *root_type_; }
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
    // Get/set the value of this GpTree with no subtrees. This "leaf value" of
    // the tree is typically a terminal constant or varible. Note that setting
    // the value requires a GpType be specified.
    std::any getLeafValue() const { return leaf_value_; }
    void setLeafValue(std::any value, const GpType& gp_type)
    {
        leaf_value_ = value;
        root_type_ = &gp_type;
    }
    // A GpTree is a "leaf node" if it has no GpFunction at its root.
    bool isLeaf() const { return !root_function_; }
    // Evaluate this tree. Run/evaluate the GpFunction at the root, recursively
    // running evaluating each parameter subtree.
    std::any eval() const
    {
        return (isLeaf() ? getLeafValue() : getFunction().eval(*this));
    }
    // Evaluate i-th subtree, corresponds to i-th parameter of root function,
    // then cast the resulting std::any to the given concrete type T.
    template <typename T> T evalSubtree(int i) const
    {
        return std::any_cast<T>(getSubtree(i).eval());
    }
    // Convert this GpTree to "source code" format as a string. It is either a
    // single constant "leaf" value, or the root function's name followed by a
    // parenthesized, comma separated, list of parameter trees.
    std::string to_string() const
    {
        std::string s;
        if (isLeaf())
        {
            s += getType().to_string(getLeafValue());
        }
        else
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
        return s;
    }
    
    // Collect all subtrees into an std::vector. Recursively traverses tree from
    // this root down, storing pointers to each subtree into vector.
    // NOTE: assumes vector is empty before initial call.
    void collectVectorOfSubtrees(std::vector<GpTree*>& all_subtrees_output)
    {
        all_subtrees_output.push_back(this);
        for (auto& subtree : subtrees())
            subtree.collectVectorOfSubtrees(all_subtrees_output);
    }

    // Collect all subtree typess into an std::set. Recursively traverses tree
    // from this root down, inserting pointers to each GpType into set.
    // NOTE: assumes set is empty before initial call.
    void collectSetOfTypes(std::set<const GpType*>& set_of_types_output) const
    {
        set_of_types_output.insert(&getType());
        for (auto& subtree : subtrees())
            subtree.collectSetOfTypes(set_of_types_output);
    }
    
    // Given two trees find the set of types common to both. That is: collect
    // the set of types for each then take the intersection of those types.
    static void sharedSetOfTypes(const GpTree& a,
                                 const GpTree& b,
                                 std::set<const GpType*>& set_of_types_output)
    {
        std::set<const GpType*> a_types;
        std::set<const GpType*> b_types;
        a.collectSetOfTypes(a_types);
        b.collectSetOfTypes(b_types);
        set_of_types_output.clear();
        std::set_intersection(a_types.begin(), a_types.end(),
                              b_types.begin(), b_types.end(),
                              std::inserter(set_of_types_output,
                                            set_of_types_output.begin()));
    }
    
    // Utility for crossover(). Select a random subtree, whose size is greater
    // then or equal to "min_size" and whose root type is in set of "types".
    // (Default to root if entire tree is not larger than "min_size". See final
    // paragraph on https://cwreynolds.github.io/LazyPredator/#20201019)
    GpTree& selectCrossoverSubtree(int min_size,
                                   const std::set<const GpType*>& types)
    {
        GpTree* result = this;
        auto gp_type_ok = [&](GpTree* tree)
            { return types.find(&(tree->getType())) != types.end(); };
        if (size() > min_size)
        {
            std::vector<GpTree*> all_subtrees;
            collectVectorOfSubtrees(all_subtrees);
            std::vector<GpTree*> filtered_subtrees;
            for (auto& gp_tree : all_subtrees)
            {
                if (gp_type_ok(gp_tree) && (gp_tree->size() >= min_size))
                {
                    filtered_subtrees.push_back(gp_tree);
                }
            }
            assert(!filtered_subtrees.empty());
            result = LPRS().randomSelectElement(filtered_subtrees);
        }
        assert(gp_type_ok(result));
        return *result;
    }

    // Traverse tree, applying "jiggle" point mutation on each constant leaf
    // value, according to it GpType.
    // TODO note makeRandomTree() and crossover() are methods of FunctionSet
    //      Is this misplaced, or are they?
    void mutate()
    {
        if (isLeaf())
        {
            setLeafValue(getType().jiggleConstant(getLeafValue()), getType());
        }
        else
        {
            for (auto& subtree : subtrees()) subtree.mutate();
        }
    }
    // Essentially like operator==() but needs to be a template for the sake of
    // the std::any leaf nodes. Used only in the unit tests.
    template <typename T> static bool match(const GpTree& a, const GpTree& b)
    {
        auto equal_subtrees = [](const GpTree& a, const GpTree& b)
        {
            bool ok = a.subtrees_.size() == b.subtrees_.size();
            for (int i = 0; i < a.subtrees_.size(); i++)
                if (!match<T>(a.getSubtree(i), b.getSubtree(i))) ok = false;
            return ok;
        };
        return (a.root_function_ == b.root_function_ && // Root functions match.
                a.root_type_ == b.root_type_ &&         // Root types match.
                a.id_ == b.id_ &&                       // IDs match.
                equal_subtrees(a, b) &&                 // All subtrees match.
                (!(a.isLeaf() && b.isLeaf()) ||         // Both not leaves, or
                 (std::any_cast<T>(a.leaf_value_) ==    //   both leaves match.
                  std::any_cast<T>(b.leaf_value_))));
    }
private:
    // NOTE: if any more data members are added, compare them in equals().
    // Add (allocate) one subtree. addSubtrees() is external API.
    void addSubtree() { subtrees_.push_back({}); }
    // Each GpTree root will have a function object or a leaf value.
    const GpFunction* root_function_ = nullptr;
    const GpType* root_type_ = nullptr;
    std::any leaf_value_;
    std::vector<GpTree> subtrees_;
    std::string id_;                            // TODO for debugging only.
};

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
        if (function_filter) function_filter(ok);
        return (ok.empty() ? nullptr : ok.at(LPRS().randomN(ok.size())));
    }

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
            gp_tree.setLeafValue(leaf_value, return_type);
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
        gp_tree.setFunction(root_function);
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
    
    // Intended to be used only for testing and debugging
    static inline std::function<void(std::vector<GpFunction*>&)>
        function_filter = nullptr;

    // Crossover: given three GpTrees (two parents and an offspring) set the
    // offspring to be a random crossover of the other two. That is: take a
    // random subtree of one, and insert it into a random subtree of the other.
    // TODO it might be better not to copy donor, but gets into const troubles.
    void crossover(const GpTree& parent0,
                   const GpTree& parent1,
                   GpTree& offspring) const
    {
        bool exchange = LPRS().frandom01() > 0.5;
        // The "donor" is a copy of one parent, selected randomly.
        GpTree donor = exchange ? parent0 : parent1;
        // The offspring is initialized to a copy of the other parent.
        offspring = exchange ? parent1 : parent0;
        // Find set of GpTypes which is common to both parents.
        std::set<const GpType*> types;
        GpTree::sharedSetOfTypes(parent0, parent1, types);
        assert(!types.empty());
        // Select random subtree from donor (=> min-size, any shared_type).
        int min_size = getCrossoverMinSize();
        GpTree& d_subtree = donor.selectCrossoverSubtree(min_size, types);
        // Select random subtree from offspring (any size, same type as dst).
        std::set<const GpType*> donor_type = {&d_subtree.getType()};
        GpTree& o_subtree = offspring.selectCrossoverSubtree(1, donor_type);
        // Set offspring subtree to copy of donor subtree.
        o_subtree = d_subtree;
    }

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
