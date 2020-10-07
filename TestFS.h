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
        ClassA(const ClassB& b, ClassC c) : b_(b), c_(c) {}
        std::string to_string() const
        {
            return ("ClassA(" + b_.to_string() + ", " + c_.to_string() + ")");
        }
    private:
        const ClassB& b_;
        const ClassC c_;
    };
private:
    static inline const FunctionSet tree_eval_objects =
    {
        {
            {
                "Float",
                [](){ return std::any(LPRS().frandom01()); },
                any_to_string<float>
            },
            {
                "Int",
                [](){ return std::any(int(LPRS().randomN(10))); },
                any_to_string<int>
            },
            {
                "ClassA",
                nullptr,
                [](std::any a)
                {
                    return std::any_cast<ClassA&>(a).to_string();
                }
            },
            {
                "ClassB",
                nullptr,
                [](std::any a)
                {
                    return std::any_cast<ClassB*>(a)->to_string();
                }
            },
            {
                "ClassC",
                nullptr,
                [](std::any a)
                {
                    return std::any_cast<ClassC>(a).to_string();
                }
            },
        },
        {
            {
                "ClassA", "ClassA", {"ClassB", "ClassC"},
                [](const GpTree& t)
                {
                    return std::any(new ClassA(*t.evalSubtree<ClassB*>(0),
                                               t.evalSubtree<ClassC>(1)));
                }
            },
            {
                "ClassB", "ClassB", {"Float"},
                [](const GpTree& t)
                {
                    return std::any(new ClassB(t.evalSubtree<float>(0)));
                }
            },
            {
                "ClassC", "ClassC", {"Int", "Int"},
                [](const GpTree& t)
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
            {
                "Float",
                [](){ return std::any(LPRS().frandom01()); },
                any_to_string<float>
            },
            {
                "Int",
                [](){ return std::any(int(LPRS().randomN(10))); },
                any_to_string<int>
            }
        },
        {
            {
                "AddInt", "Int", {"Int", "Int"}, [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "AddFloat", "Float", {"Float", "Float"}, [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<float>(0) +
                                    t.evalSubtree<float>(1));
                }
            },
            {
                "Floor", "Int", {"Float"}, [](const GpTree& t)
                {
                    return std::any(int(std::floor(t.evalSubtree<float>(0))));
                }
            },
            {
                "Sqrt", "Float", {"Int"}, [](const GpTree& t)
                {
                    return std::any(float(std::sqrt(t.evalSubtree<int>(0))));
                }
            },
            {
                "Mult", "Float", {"Float", "Int"}, [](const GpTree& t)
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
            {
                "Int",
                [](){ return std::any(int(LPRS().randomN(10))); },
                any_to_string<int>
            }
        },
        {
            {
                "P", "Int", {"Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0));
                }
            },
            {
                "PP", "Int", {"Int", "Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "PPP", "Int", {"Int", "Int", "Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1) +
                                    t.evalSubtree<int>(2));
                }
            },
            {
                "Q", "Int", {"Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0));
                }
            },
            {
                "QQ", "Int", {"Int", "Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1));
                }
            },
            {
                "QQQ", "Int", {"Int", "Int", "Int"},
                [](const GpTree& t)
                {
                    return std::any(t.evalSubtree<int>(0) +
                                    t.evalSubtree<int>(1) +
                                    t.evalSubtree<int>(2));
                }
            }
        }
    };
};
