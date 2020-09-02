//
//  TexSynTemp.cpp
//  LazyPredator
//
//  Created by Craig Reynolds on 8/14/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "TexSynTemp.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO prototyping "template GpType" -- connect GpType to c++ types

// Say this is the underlying c++ type (can also be eg float, int, etc.)
class Foo { public: Foo(){} };

std::ostream& operator<<(std::ostream &os, const Foo& f)
{
    return os << "<Foo " << ((void*)&f) << ">";
}


// The current "unconnected" type :
GpType FooOld("Foo");

// The prototype "connected" type would be something like:
TestGpType<Foo> FooObject("FooObject", [](){ return Foo(); });
TestGpType<Foo*> FooPointer("FooPointer", [](){ return new Foo(); });
TestGpType<Foo&> FooReference("FooReference",
                              [](){ return std::ref(*(new Foo())); });
TestGpType<int> Int_1_10("Int_1_10", [](){ return rand() % 10; });
TestGpType<float> Float01("Float01", [](){ return frandom01(); });

std::vector<GpType*> gp_types =
    {&FooOld, &FooObject, &FooPointer, &FooReference, &Int_1_10, &Float01};

void TestTemplateGpType()
{
    std::cout << std::endl << "name()" << std::endl;
    for (auto& t : gp_types) debugPrint(t->name());
    std::cout << std::endl << "hasEphemeralGenerator()" << std::endl;
    for (auto& t : gp_types)
    {
        TestGpType<float>& u = static_cast<TestGpType<float>&>(*t);
        debugPrint(u.hasEphemeralGenerator());
    }
//    std::cout << std::endl << "hasEphemeralGenerator()" << std::endl;
//    for (auto& t : gp_types)
//    {
//        auto& u = static_cast<TestGpType<float>&>(*t);
//        if (u.hasEphemeralGenerator())
//            debugPrint(u.generateEphemeralConstant());
//    }
    
    std::cout << std::endl;
    auto& a = static_cast<TestGpType<Foo>&>(*(gp_types.at(1)));
    auto& b = static_cast<TestGpType<Foo*>&>(*(gp_types.at(2)));
    auto& c = static_cast<TestGpType<Foo&>&>(*(gp_types.at(3)));
    auto& d = static_cast<TestGpType<int>&>(*(gp_types.at(4)));
    auto& e = static_cast<TestGpType<float>&>(*(gp_types.at(5)));
    
    std::cout << a.name() << ": " << a.generateEphemeralConstant() << std::endl;
    std::cout << b.name() << ": " << b.generateEphemeralConstant() << std::endl;
    std::cout << c.name() << ": " << c.generateEphemeralConstant() << std::endl;
    std::cout << d.name() << ": " << d.generateEphemeralConstant() << std::endl;
    std::cout << e.name() << ": " << e.generateEphemeralConstant() << std::endl;


}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO very temporary prototyping

// Generate a value for an ephemeral constant, given a GpType and a c++ type.
//
// TODO This could be a member of GpType.
//      But the problem remains: which concrete type is the template parameter?
// TODO could indicate "none" by returning a std::vector<T> of 0 or 1 elements
//
template<typename T>
T generate_ephemeral_constant(GpType& gp_type)
{
    auto& gp_type_template = static_cast<TestGpType<T>&>(gp_type);
    return (gp_type_template.hasEphemeralGenerator() ?
            gp_type_template.generateEphemeralConstant() :
            T());
};

void VeryTemporaryPrototyping()
{
    debugPrint("VeryTemporaryPrototyping");
    
    // debugPrint(generate_ephemeral_constant<FooOld>(*gp_types.at(0)));
    debugPrint(generate_ephemeral_constant<Foo>(*gp_types.at(1)));
    debugPrint(generate_ephemeral_constant<Foo*>(*gp_types.at(2)));
    // debugPrint(generate_ephemeral_constant<Foo&>(*gp_types.at(3)));
    debugPrint(generate_ephemeral_constant<int>(*gp_types.at(4)));
    debugPrint(generate_ephemeral_constant<float>(*gp_types.at(5)));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
