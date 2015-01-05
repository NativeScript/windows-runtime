#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(ClassesTests) {
public:
    TEST_METHOD(EmptyClass) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EmptyClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"EmptyClass");
        Assert::IsTrue(declaration->fullName() == name);
        Assert::IsTrue(declaration->baseFullName() == L"System.Object");
    }

    TEST_METHOD(ImplementedInterfaces) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::InterfaceIterator> interfaces{declaration->implementedInterfaces()};
        ClassDeclaration::InterfaceIterator it{interfaces.begin()};

        const unique_ptr<const InterfaceDeclaration>& implementedInterface1{*it};
        Assert::IsTrue(implementedInterface1->fullName() == L"NativeScript.Tests.Fixtures.ISimpleClassClass");

        ++it;
        const unique_ptr<const InterfaceDeclaration>& implementedInterface2{*it};
        Assert::IsTrue(implementedInterface2->fullName() == L"Windows.Foundation.IStringable");

        ++it;
        Assert::IsTrue(it == interfaces.end());
    }

    TEST_METHOD(Methods) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::MethodIterator> methods{declaration->methods()};
        ClassDeclaration::MethodIterator it{methods.begin()};

        const MethodDeclaration& staticMethod{*it};
        Assert::IsTrue(staticMethod.name() == L"StaticMethod");
        Assert::IsTrue(staticMethod.fullName() == L"StaticMethod");
        Assert::IsTrue(staticMethod.isStatic() == true);
        Assert::IsTrue(staticMethod.isSealed() == true);

        ++it;
        const MethodDeclaration& instanceMethod{*it};
        Assert::IsTrue(instanceMethod.name() == L"InstanceMethod");
        Assert::IsTrue(instanceMethod.fullName() == L"InstanceMethod");
        Assert::IsTrue(instanceMethod.isStatic() == false);
        Assert::IsTrue(instanceMethod.isSealed() == true);

        ++it;
        Assert::IsTrue(it == methods.end());
    }

    TEST_METHOD(OverrideableMethods) {
        MetadataReader metadataReader;

        const wchar_t* name{L"Windows.UI.Xaml.Application"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        MethodDeclaration overridableMethod{declaration->findMethodsWithName(L"OnActivated")[0]};
        Assert::IsTrue(overridableMethod.isSealed() == false);
    }

    TEST_METHOD(Properties) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::PropertyIterator> properties{declaration->properties()};
        ClassDeclaration::PropertyIterator it{properties.begin()};

        const PropertyDeclaration& staticProperty{*it};
        Assert::IsTrue(staticProperty.name() == L"StaticProperty");
        Assert::IsTrue(staticProperty.fullName() == L"StaticProperty");
        Assert::IsTrue(staticProperty.getter().name() == L"get_StaticProperty");
        Assert::IsTrue(staticProperty.setter()->name() == L"put_StaticProperty");
        Assert::IsTrue(staticProperty.isStatic() == true);
        Assert::IsTrue(staticProperty.isSealed() == true);

        ++it;
        const PropertyDeclaration& staticReadonlyProperty{*it};
        Assert::IsTrue(staticReadonlyProperty.name() == L"StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.fullName() == L"StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.getter().name() == L"get_StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(staticReadonlyProperty.isStatic() == true);
        Assert::IsTrue(staticReadonlyProperty.isSealed() == true);

        ++it;
        const PropertyDeclaration& instanceProperty{*it};
        Assert::IsTrue(instanceProperty.name() == L"InstanceProperty");
        Assert::IsTrue(instanceProperty.fullName() == L"InstanceProperty");
        Assert::IsTrue(instanceProperty.getter().name() == L"get_InstanceProperty");
        Assert::IsTrue(instanceProperty.setter()->name() == L"put_InstanceProperty");
        Assert::IsTrue(instanceProperty.isStatic() == false);

        ++it;
        const PropertyDeclaration& instanceReadonlyProperty{*it};
        Assert::IsTrue(instanceReadonlyProperty.name() == L"InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.fullName() == L"InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.getter().name() == L"get_InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(instanceReadonlyProperty.isStatic() == false);

        ++it;
        Assert::IsTrue(it == properties.end());
    }

    TEST_METHOD(Initializers) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> activatableClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.ActivatableClass"))};

        IteratorRange<ClassDeclaration::MethodIterator> initializers{activatableClass->initializers()};

        ClassDeclaration::MethodIterator it{initializers.begin()};
        const MethodDeclaration& initializer0{*it};
        Assert::IsTrue(initializer0.numberOfParameters() == 0);
        size_t index0{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory0{findDeclaringInterfaceForMethod(initializer0, &index0)};
        Assert::IsTrue(initializerFactory0 == nullptr);
        Assert::IsTrue(index0 == SIZE_T_MAX);

        ++it;
        const MethodDeclaration& initializer1{*it};
        Assert::IsTrue(initializer1.numberOfParameters() == 1);
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory1{findDeclaringInterfaceForMethod(initializer1, &index1)};
        Assert::IsTrue(initializerFactory1->id() == IID{0xE1C8B4D2, 0xCA7C, 0x5558,{0x7B, 0x6C, 0xAB, 0xC7, 0x47, 0xB8, 0x71, 0x3B}});
        Assert::IsTrue(index1 == 0);

        ++it;
        const MethodDeclaration& initializer2{*it};
        Assert::IsTrue(initializer2.numberOfParameters() == 2);
        size_t index2{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory2{findDeclaringInterfaceForMethod(initializer2, &index2)};
        Assert::IsTrue(initializerFactory2->id() == IID{0xE1C8B4D2, 0xCA7C, 0x5558,{0x7B, 0x6C, 0xAB, 0xC7, 0x47, 0xB8, 0x71, 0x3B}});
        Assert::IsTrue(index2 == 1);

        ++it;
        const MethodDeclaration& initializer3{*it};
        Assert::IsTrue(initializer3.numberOfParameters() == 3);
        size_t index3{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory3{findDeclaringInterfaceForMethod(initializer3, &index3)};
        Assert::IsTrue(initializerFactory3->id() == IID{0x85A3BF7F, 0xBBAA, 0x5DD4,{0x78, 0x96, 0x3D, 0x18, 0x5B, 0xFA, 0xA6, 0x3C}});
        Assert::IsTrue(index3 == 0);

        ++it;
        const MethodDeclaration& initializer4{*it};
        Assert::IsTrue(initializer4.numberOfParameters() == 4);
        size_t index4{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory4{findDeclaringInterfaceForMethod(initializer4, &index4)};
        Assert::IsTrue(initializerFactory4->id() == IID{0x85A3BF7F, 0xBBAA, 0x5DD4,{0x78, 0x96, 0x3D, 0x18, 0x5B, 0xFA, 0xA6, 0x3C}});
        Assert::IsTrue(index4 == 1);

        ++it;
        const MethodDeclaration& initializer5{*it};
        Assert::IsTrue(initializer5.numberOfParameters() == 5);
        size_t index5{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory5{findDeclaringInterfaceForMethod(initializer5, &index5)};
        Assert::IsTrue(initializerFactory5->id() == IID{0xF0215C81, 0xD4DC, 0x53B3,{0x77, 0xFE, 0x59, 0xE4, 0x06, 0x6B, 0x4C, 0x00}});
        Assert::IsTrue(index5 == 0);

        ++it;
        Assert::IsTrue(it == initializers.end());
    }

    TEST_METHOD(ClassType) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> emptyClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.EmptyClass"))};
        Assert::IsTrue(emptyClass->isInstantiable() == false);
        Assert::IsTrue(emptyClass->isSealed() == true);

        shared_ptr<ClassDeclaration> activatableClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.ActivatableClass"))};
        Assert::IsTrue(activatableClass->isInstantiable() == true);
        Assert::IsTrue(activatableClass->isSealed() == true);

        shared_ptr<ClassDeclaration> composableClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.Controls.ListView"))};
        Assert::IsTrue(composableClass->isInstantiable() == true);
        Assert::IsTrue(composableClass->isSealed() == false);

        shared_ptr<ClassDeclaration> abstractClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.Controls.Control"))};
        Assert::IsTrue(abstractClass->isInstantiable() == false);
        Assert::IsTrue(abstractClass->isSealed() == false);
    }

    // TEST_METHOD(StaticAttribute) {
    //     MetadataReader metadataReader;
    //
    //     shared_ptr<ClassDeclaration> emptyClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.EmptyClass"))};
    //     Assert::IsTrue(emptyClass->staticInterface() == nullptr);
    //
    //     shared_ptr<ClassDeclaration> simpleClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.SimpleClass"))};
    //     Assert::IsTrue(simpleClass->staticInterface()->fullName() == L"NativeScript.Tests.Fixtures.ISimpleClassStatic");
    //     Assert::IsTrue(simpleClass->staticInterface()->id() == IID{0xF07E1852, 0xC1F3, 0x5BE8,{0x5A, 0x6F, 0x6A, 0xC5, 0x36, 0xDE, 0x8E, 0xEB}});
    // }
};

}
}
