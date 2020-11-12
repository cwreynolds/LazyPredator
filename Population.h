//
//  Population.h
//  LazyPredator
//
//  Created by Craig Reynolds on 8/6/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once

#include "Individual.h"
#include "FunctionSet.h"

class TournamentGroup;
class TournamentGroupMember;

// Classes to represent the Individuals participating in a tournament. Each one
// is a TournamentGroupMember composed of an Individual pointer, an index in the
// Population, and an optional float fitness metric. The collection of those, a
// TournamentGroup, is passed around functions related to tournaments. The
// members of a group are kept in sorted order.
// TODO maybe move to their own file rather than be distracting clutter here?

class TournamentGroupMember
{
public:
    TournamentGroupMember()
        : individual(nullptr), index(0), metric(0) {}
    TournamentGroupMember(Individual* individual_, int index_)
        : individual(individual_), index(index_), metric(0) {}
    TournamentGroupMember(Individual* individual_, int index_, float metric_)
        : individual(individual_), index(index_), metric(metric_) {}
    Individual* individual = nullptr;  // pointer to an Individual.
    int index = 0;                     // Individual's index in Population.
    float metric = 0;                  // Optional fitness metric.
};

class TournamentGroup
{
public:
    TournamentGroup() { sort(); }
    TournamentGroup(const std::vector<TournamentGroupMember>& member_list)
        : members_(member_list) { sort(); }
    // Const reference to vector of members. (TODO any need for this?)
    const std::vector<TournamentGroupMember>& members() const {return members_;}
    // Number of members in this group (normally 3).
    size_t size() const { return members().size(); }
    // For "numerical fitness"-based tournaments, map a given scoring function
    // over all members to set the metric values. Sorts members afterward.
    void setAllMetrics(std::function<float(Individual*)> scoring)
    {
        for (auto& m : members_) { m.metric = scoring(m.individual); }
        sort();
    }
    int worstIndex() const { return members().front().index; }
    Individual* worstIndividual() const { return members().front().individual; }
    Individual* bestIndividual() const { return members().back().individual; }
    Individual* secondBestIndividual() const
    {
        return members().at(size() - 2).individual;
    }
    // For debugging/testing. TODO or should this be to_string() ?
    void print() const
    {
        // TODO need a template utility for printing an std::vector as a comma
        //      separated list.
        std::cout << "TournamentGroup: {";
        bool first = true;
        for (auto& m : members())
        {
            if (!first) { std::cout << ", "; first = false; }
            std::cout << "{" << m.individual << ", ";
            std::cout << m.index << ", " << m.metric << "}";
        }
        std::cout << "}" << std::endl;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Find largest tree size of all Individuals in this TournamentGroup.
    int maxTreeSize() const
    {
        int max_tree_size = std::numeric_limits<int>::min();
        for (auto& m : members())
        {
            int tree_size = m.individual->tree().size();
            if (max_tree_size < tree_size) max_tree_size = tree_size;
        }
        return max_tree_size;
    }
    
    // TODO maybe this is better suited to needs of CWE?
    // Find smallest tree size of all Individuals in this TournamentGroup.
    int minTreeSize() const
    {
//        int max_tree_size = std::numeric_limits<int>::min();
        int min_tree_size = std::numeric_limits<int>::max();
        for (auto& m : members())
        {
            int tree_size = m.individual->tree().size();
//            if (max_tree_size < tree_size) max_tree_size = tree_size;
            if (min_tree_size > tree_size) min_tree_size = tree_size;
        }
//        return max_tree_size;
        return min_tree_size;
    }

    // Given an Individual, returns an int from 1 to members().size(),
    // rank 1 corresponds to bestIndividual().
    int rankOfIndividual(Individual* individual)
    {
        int rank = 0;
        int count = int(members().size());
        for (int i = 0; i <  count; i++)
        {
            if (individual == members().at(i).individual) rank = count - i;
        }
        assert("given Individual not in TournamentGroup" && rank != 0);
        return rank;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    // Sort the members of this group by their "metric" value.
    void sort()
    {
        auto sorted = [](const TournamentGroupMember &a,
                         const TournamentGroupMember &b)
                        { return a.metric < b.metric; };
        std::sort(members_.begin(), members_.end(), sorted);
    }
    std::vector<TournamentGroupMember> members_;
};

class Population
{
public:
    Population() {}
    Population(int size)
    {
        for (int i = 0; i < size; i++) individuals_.push_back(new Individual);
    }
    // TODO combine constructors with and without fs?
    Population(int size, int max_tree_size, const FunctionSet& fs)
    {
        for (int i = 0; i < size; i++)
        {
            individuals_.push_back(new Individual(max_tree_size, fs));
        }
    }
    ~Population()
    {
        while (!individuals_.empty())
        {
            Individual* last = individuals_.back();
            individuals_.pop_back();
            delete last;
        }
    }
    // Functions that implement tournaments, by transforming a TournamentGroup.
    typedef std::function<TournamentGroup(TournamentGroup)> TournamentFunction;
    // Return const reference to collection of Individuals in Population.
    const std::vector<Individual*>& individuals() const { return individuals_; }
    // Perform one step of the "steady state" evolutionary computation. Hold a
    // tournament with three randomly selected Individuals. The "loser" is
    // replaced in the Population by a new "offspring" created by crossing over
    // the two "winners" and mutating the result
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        TournamentGroup random_group = selectTournamentGroup();
        // Run tournament amoung the three, return ranked group.
        TournamentGroup ranked_group = tournament_function(random_group);
        Individual* loser = ranked_group.worstIndividual();
        int loser_index = ranked_group.worstIndex();
        assert(loser);
        // Other two become parents of new offspring.
        Individual* parent_0 = ranked_group.secondBestIndividual();
        Individual* parent_1 = ranked_group.bestIndividual();
        // Both parent's rank increases because they survived the tournament.
        parent_0->incrementTournamentsSurvived();
        parent_1->incrementTournamentsSurvived();
        // Create new offspring tree by crossing-over these two parents.
        GpTree new_tree;
        function_set.crossover(parent_0->tree(), parent_1->tree(), new_tree);
        // Mutate constants in new tree.
        new_tree.mutate();
        // Create new offspring Individual from new tree.
        Individual* offspring = new Individual(new_tree);
        // Delete tournament loser from Population, replace with new offspring.
        replaceIndividual(loser_index, offspring);
        // TODO TEMP for debugging
        last_individual_added = offspring;
    }

    // TODO TEMP for debugging
    static inline Individual* last_individual_added = nullptr;
    
    // Delete Individual at index i, then overwrite pointer with replacement.
    // TODO merge this with code now in ~Population()
    void replaceIndividual(int i, Individual* new_individual)
    {
        delete individuals_.at(i);
        individuals_.at(i) = new_individual;
    }
    // TournamentGroup with three Individuals selected randomly from Population.
    TournamentGroup selectTournamentGroup()
    {
        auto [i, j, k] = selectThreeIndices();
        return TournamentGroup({ {individual(i), i},
                                 {individual(j), j},
                                 {individual(k), k} });
    }
    // Select three random (but guaranteed to be unique) indices into the
    // population for Individuals to be used in a three way tournament.
    // (TODO later: is there really ANY advantage to ensuring the indices
    //       are unique? Say they were all the same number. So what?)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // TODO was thinking about flat/uniform selection, and given that comment
    // above (who cares about uniqueness?) here is a new version of this which
    // does 3x "tournament" selection based on tournament survivial.
    
//    std::tuple<int, int, int> selectThreeIndices()
//    {
//        int count = static_cast<int>(individuals().size()) - 1;
//        assert("fewer than 3 in population" && (count >= 2));
//        int one_third = count / 3;
//        int two_thirds = (count * 2) / 3;
//        return std::make_tuple(LPRS().random2(0, one_third),
//                               LPRS().random2(one_third + 1, two_thirds),
//                               LPRS().random2(two_thirds + 1, count));
//    };
  
    // Consider three random Individuals from Population, return the one with
    // the largest "tournaments survived".
    int randomIndexFromTournamentsSurvival() const
    {
        auto ri = [&](){ return LPRS().randomN(individuals().size()); };
        std::vector<int> indices = { ri(), ri(), ri() };
        auto i2ts = [&](int i)
            { return individuals().at(i)->getTournamentsSurvived(); };
        auto most_survived = [&](int a, int b) { return i2ts(a) > i2ts(b); };
//        debugPrint(vec_to_string(indices));
        std::sort(indices.begin(), indices.end(), most_survived);
//        debugPrint(vec_to_string(indices));
//        debugPrint(i2ts(indices.at(0)));
//        debugPrint(i2ts(indices.at(1)));
//        debugPrint(i2ts(indices.at(2)));
        return indices.front();
    }
    
    std::tuple<int, int, int> selectThreeIndices()
    {
        return std::make_tuple(randomIndexFromTournamentsSurvival(),
                               randomIndexFromTournamentsSurvival(),
                               randomIndexFromTournamentsSurvival());
    };

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Find the best Individual in Population, defined as having survived the
    // most tournaments. TODO name?
    Individual* findBestIndividual() const
    {
        assert(!individuals().empty()); // Or just return nullptr in this case?
        int most_survived = std::numeric_limits<int>::min();
        Individual* best_individual = nullptr;
        for (auto& individual : individuals())
        {
            int survived = individual->getTournamentsSurvived();
            if (most_survived < survived)
            {
                most_survived = survived;
                best_individual = individual;
            }
        }
        return best_individual;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Returns (by value, eg copied) a vector of "n" Individual* pointers having
    // the largest values of getTournamentsSurvived(). These are, in some sense,
    // the "best" n Individuals in the population.
//    std::vector<const Individual*> nMostTournamentsSurvived(int n)
    std::vector<Individual*> nMostTournamentsSurvived(int n)
    {
        std::vector<Individual*> collection = individuals();
        auto best_survior = [](Individual* a, Individual* b)
            {return a->getTournamentsSurvived() > b->getTournamentsSurvived();};
        std::sort(collection.begin(), collection.end(), best_survior);
        if (n < individuals().size()) { collection.resize(n); }
        return collection;
    }
    
//    std::cout << "pop ave size=" << population->averageTreeSize();
//    std::cout << " won=" << population->averageTournamentsSurvived();

    // Average of "tree size" over all Individuals.
    int averageTreeSize() const
    {
        int total = 0;
        for (auto& i : individuals()) { total += i->tree().size(); }
        return total / individuals().size();
    }

    // Average of "tournaments survived" over all Individuals.
    float averageTournamentsSurvived() const
    {
        int total = 0;
        for (auto& i : individuals()) { total += i->getTournamentsSurvived(); }
        return float(total) / individuals().size();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Run "steps" of evolution, given "function_set" and "tournament_function".
    void run(int steps,
             const FunctionSet& function_set,
             TournamentFunction tournament_function)
    {
        // Run given number of steps.
        for (int i = 0; i < steps; i++)
        {
            // Run evolution step with given tournament and function set.
            evolutionStep(tournament_function, function_set);
        }
    }
private:
    Individual* individual(int i) { return individuals_.at(i); }
    std::vector<Individual*> individuals_;
};
