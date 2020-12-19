//
//  TestFS.h
//  LazyPredator
//
//  Created by Craig Reynolds on 9/10/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

//  Sample FunctionSets for testing and as examples.

#pragma once
#include "FunctionSet.h"

class TestFS
{
public:
    // Simple test with only Float and Int types which can be evaluated.
    static const FunctionSet& treeEval() { return tree_eval; }
    // For testing tree eval for cases including construction class objects.
    static const FunctionSet& treeEvalObjects() { return tree_eval_objects; }
    // Simple set for testing crossover.
    static const FunctionSet& crossover() { return cross_over; }

    class ClassC
    {
    public:
        ClassC(int i, int j) : i_(i), j_(j) {}
        std::string to_string() const
        {
            return ("ClassC(" + std::to_string(i_) +
                    ", " + std::to_string(j_) + ")");
        }
    private:
        int i_;
        int j_;
    };
    class ClassB
    {
    public:
        ClassB(float f) : f_(f) {}
        std::string to_string() const
        {
            return "ClassB(" + any_to_string<float>(f_) + ")";
        }
    private:
        float f_;
    };
    class ClassA
    {
    public:
        ClassA(const ClassB& b, ClassC c) : b_(b), c_(c)
        {
            constructor_count_++;
        }
        ~ClassA()
        {
            destructor_count_++;
        }
        static int getLeakCount()
        {
            return constructor_count_ - destructor_count_;
        }
        std::string to_string() const
        {
            return ("ClassA(" + b_.to_string() + ", " + c_.to_string() + ")");
        }
    private:
        const ClassB& b_;
        const ClassC c_;
        // Leak check. Count constructor/destructor calls. Should match at exit.
        static inline int constructor_count_ = 0;
        static inline int destructor_count_ = 0;
    };
private:
    static inline const FunctionSet tree_eval_objects =
    {
        {
            {
                "ClassA",
                // ephemeral generator
                nullptr,
                // to_string
                [](std::any a)
                {
                    return std::any_cast<ClassA&>(a).to_string();
                },
                // jiggler
                nullptr,
                // deleter
                [](std::any a)
                {
                    if (a.has_value())
                    {
                        ClassA* t = std::any_cast<ClassA*>(a);
                        if (t) delete t;
                    }
                }
            },
            {
                "ClassB",
                nullptr,
                [](std::any a)
                {
                    return std::any_cast<ClassB*>(a)->to_string();
                },
                nullptr
            },
            {
                "ClassC",
                nullptr,
                [](std::any a)
                {
                    return std::any_cast<ClassC>(a).to_string();
                },
                nullptr
            },
            { "Float", 0.0f, 1.0f },
            { "Int", 0, 9 }
        },
        {
            {
                "ClassA", "ClassA", {"ClassB", "ClassC"},
                [](GpTree& t)
                {
                    return std::any(new ClassA(*t.evalSubtree<ClassB*>(0),
                                               t.evalSubtree<ClassC>(1)));
                }
            },
            {
                "ClassB", "ClassB", {"Float"},
                [](GpTree& t)
                {
                    return std::any(new ClassB(t.evalSubtree<float>(0)));
                }
            },
            {
                "ClassC", "ClassC", {"Int", "Int"},
                [](GpTree& t)
                {
                    return std::any(ClassC(t.evalSubtree<int>(0),
                                           t.evalSubtree<int>(1)));
                }
            }
        }
    };

    // Moved "Float" to top in case we want to use that convention.
    // std::string root_type = "Float";
    static inline const FunctionSet tree_eval =
    {
        {
            { "Float", 0.0f, 1.0f },
            { "Int", 0, 9 }
        },
        {
            {
                "AddInt", "Int", {"Int", "Int"}, [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "AddFloat", "Float", {"Float", "Float"}, [](GpTree& t)
                {
                    return std::any(t.evalSubtree<float>(0) +
                                    t.evalSubtree<float>(1));
                }
            },
            {
                "Floor", "Int", {"Float"}, [](GpTree& t)
                {
                    return std::any(int(std::floor(t.evalSubtree<float>(0))));
                }
            },
            {
                "Sqrt", "Float", {"Int"}, [](GpTree& t)
                {
                    return std::any(float(std::sqrt(t.evalSubtree<int>(0))));
                }
            },
            {
                "Mult", "Float", {"Float", "Int"}, [](GpTree& t)
                {
                    return std::any(t.evalSubtree<float>(0) *
                                    t.evalSubtree<int>(1));
                }
            }
        }
    };
        
    static inline const FunctionSet cross_over =
    {
        {
            { "Int", 0, 9 }
        },
        {
            {
                "P", "Int", {"Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0));
                }
            },
            {
                "PP", "Int", {"Int", "Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "PPP", "Int", {"Int", "Int", "Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1) +
                                    t.evalSubtree<int>(2));
                }
            },
            {
                "Q", "Int", {"Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0));
                }
            },
            {
                "QQ", "Int", {"Int", "Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "QQQ", "Int", {"Int", "Int", "Int"},
                [](GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1) +
                                    t.evalSubtree<int>(2));
                }
            }
        }
    };
};
