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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define USE_TOURNAMENT_GROUP

//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
#else // USE_TOURNAMENT_GROUP
#endif // USE_TOURNAMENT_GROUP
//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~

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
    TournamentGroup() {}
    TournamentGroup(const std::vector<TournamentGroupMember>& member_list)
        : members_(member_list) {}
    const std::vector<TournamentGroupMember>& members() const { return members_;}
    size_t size() const { return members().size(); }
    TournamentGroupMember at(int i) const { return members().at(i); }
    
    // Tournament function may want to record a metric, related to fitness.
    void setMetric(int i, float m) { members_.at(i).metric = m; }

    // TODO these will only be correct if sort() had been called since last
    // modification. Can it just assume that is the case? Should this call
    // sort() just to be sure? Some kind of flag for caching?
    Individual* bestIndividual() const { return members().back().individual; }
    Individual* worstIndividual() const { return members().front().individual; }

    void sort()
    {
        std::sort(members_.begin(),
                  members_.end(),
                  []
                  (const TournamentGroupMember &a,
                   const TournamentGroupMember &b)
                  { return a.metric < b.metric; });
    }
    
    // TODO or should this be to_string() ?
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

private:
    std::vector<TournamentGroupMember> members_;
};

// TODO super temp compilation tests
inline void ignore_me_delete_me()
{
    // TODO super temp compilation tests
    Individual ind0;
    TournamentGroupMember tgm1;
    TournamentGroupMember tgm2(&ind0, 5);
    TournamentGroupMember tgm3(&ind0, 5, 1.0);

    TournamentGroup tg1;
    TournamentGroup tg2({ {}, {}, {}, {}, {} });
    TournamentGroup tg3({ {&ind0, 5}, {&ind0, 4}, {&ind0, 3} });
    TournamentGroup tg4({ {&ind0, 5, 1.234 }, {&ind0, 4, 5.678} });
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
    // Type for functions that implement 3-way tournaments, returning the loser.
    typedef std::function<Individual*(Individual*, Individual*, Individual*)>
            TournamentFunction;
#else // USE_TOURNAMENT_GROUP
    // Functions that implement tournaments, by transforming a TournamentGroup.
    typedef std::function<TournamentGroup(TournamentGroup)> TournamentFunction;
#endif // USE_TOURNAMENT_GROUP
    //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
    // Return const reference to collection of Individuals in Population.
    const std::vector<Individual*>& individuals() const { return individuals_; }
    // Perform one step of the "steady state" evolutionary computation. Hold a
    // tournament with three randomly selected Individuals. The "loser" is
    // replaced in the Population by a new "offspring" created by crossing over
    // the two "winners" and mutating the result
    void evolutionStep(TournamentFunction tournament_function,
                       const FunctionSet& function_set)
    {
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
        // Select three Individual uniformly distributed across this Population.
        auto [i, j, k] = selectThreeIndices();
        Individual* a = individual(i);
        Individual* b = individual(j);
        Individual* c = individual(k);
        // Run tournament amoung the three, determined which one is the worst.
        Individual* loser = tournament_function(a, b, c);
        assert(loser);
        // Determine the other two which become parents of new offspring.
        // TODO this seems awkward, think of a better way:
        int loser_index = k;
        Individual* parent_0 = a;
        Individual* parent_1 = b;
        if (loser == a) { loser_index = i; parent_0 = b; parent_1 = c; }
        // TODO old bug!! this should have been "loser_index = j;"!!!!!!!!!!!!!!
        if (loser == b) { loser_index = i; parent_0 = a; parent_1 = c; }
#else // USE_TOURNAMENT_GROUP
        TournamentGroup random_group = selectTournamentGroup();
        // Run tournament amoung the three, return ranked group.
        TournamentGroup ranked_group = tournament_function(random_group);
        Individual* loser = ranked_group.worstIndividual();
        assert(loser);
        // Determine the other two which become parents of new offspring.
//        int loser_index = ranked_group.at(2).index;
//        Individual* parent_0 = ranked_group.at(0).individual;
//        Individual* parent_1 = ranked_group.at(1).individual;
        
        // TODO this was the bug Oct 29 bug, we need less error-prone API
        // (what about bestIndividual() AND secondBestIndividual()?)
        int loser_index = ranked_group.at(0).index;
        Individual* parent_0 = ranked_group.at(1).individual;
        Individual* parent_1 = ranked_group.at(2).individual;
        
#endif // USE_TOURNAMENT_GROUP
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~

        // Both parents increase their rank because they survived the tournament
        parent_0->incrementTournamentsSurvived();
        parent_1->incrementTournamentsSurvived();
        // Create new offspring tree from crossover between two parents.
        GpTree offspring_tree;
        function_set.crossover(parent_0->tree(),
                               parent_1->tree(),
                               offspring_tree);
        // Mutate constants in new tree.
        offspring_tree.mutate();
        // Create new offspring Individual from new tree.
        Individual* offspring = new Individual(offspring_tree);
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // select tournament group...
    // Select three Individual uniformly distributed across this Population.

//    TournamentGroup selectTournamentGroup()
//    {
//        auto [i, j, k] = selectThreeIndices();
//        Individual* a = individual(i);
//        Individual* b = individual(j);
//        Individual* c = individual(k);
//        return TournamentGroup({ {a, i}, {b, j}, {c, k} });
//    }

//    TournamentGroup selectTournamentGroup()
//    {
//        auto [i, j, k] = selectThreeIndices();
//        return TournamentGroup
//        ({
//            {individual(i), i},
//            {individual(j), j},
//            {individual(k), k}
//        });
//    }

//    TournamentGroup selectTournamentGroup()
//    {
//        auto [i, j, k] = selectThreeIndices();
//        return TournamentGroup
//        ({ {individual(i), i}, {individual(j), j}, {individual(k), k} });
//    }

    TournamentGroup selectTournamentGroup()
    {
        auto [i, j, k] = selectThreeIndices();
        return TournamentGroup({ {individual(i), i},
                                 {individual(j), j},
                                 {individual(k), k} });
    }

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Select three random (but guaranteed to be unique) indices into the
    // population for Individuals to be used in a three way tournament.
    // (TODO later: is there really ANY advantage to ensuring the indices
    //       are unique? Say they were all the same number. So what?)
    std::tuple<int, int, int> selectThreeIndices()
    {
        int count = static_cast<int>(individuals().size()) - 1;
        assert("fewer than 3 in population" && (count >= 2));
        int one_third = count / 3;
        int two_thirds = (count * 2) / 3;
        return std::make_tuple(LPRS().random2(0, one_third),
                               LPRS().random2(one_third + 1, two_thirds),
                               LPRS().random2(two_thirds + 1, count));
    };
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
