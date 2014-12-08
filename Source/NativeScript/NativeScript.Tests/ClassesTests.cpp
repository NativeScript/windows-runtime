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

    TEST_METHOD(Methods) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::MethodIterator> methods{declaration->methods()};
        ClassDeclaration::MethodIterator it{methods.begin()};

        MethodDeclaration staticMethod = *it;
        Assert::IsTrue(staticMethod.name() == L"StaticMethod");
        Assert::IsTrue(staticMethod.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.StaticMethod");
        Assert::IsTrue(staticMethod.isStatic() == true);

        ++it;
        MethodDeclaration instanceMethod = *it;
        Assert::IsTrue(instanceMethod.name() == L"InstanceMethod");
        Assert::IsTrue(instanceMethod.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.InstanceMethod");
        Assert::IsTrue(instanceMethod.isStatic() == false);

        ++it;
        Assert::IsTrue(it == methods.end());
    }

    TEST_METHOD(Properties) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::PropertyIterator> properties{declaration->properties()};
        ClassDeclaration::PropertyIterator it{properties.begin()};

        PropertyDeclaration staticReadonlyProperty = *it;
        Assert::IsTrue(staticReadonlyProperty.name() == L"StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.getter()->name() == L"get_StaticReadonlyProperty");
        Assert::IsTrue(staticReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(staticReadonlyProperty.isStatic() == true);

        ++it;
        PropertyDeclaration instanceReadonlyProperty = *it;
        Assert::IsTrue(instanceReadonlyProperty.name() == L"InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.fullName() == L"NativeScript.Tests.Fixtures.SimpleClass.InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.getter()->name() == L"get_InstanceReadonlyProperty");
        Assert::IsTrue(instanceReadonlyProperty.setter() == nullptr);
        Assert::IsTrue(instanceReadonlyProperty.isStatic() == false);

        ++it;
        Assert::IsTrue(it == properties.end());
    }

    TEST_METHOD(StaticAttribute) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> emptyClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.EmptyClass"))};
        Assert::IsTrue(emptyClass->staticInterface() == nullptr);

        shared_ptr<ClassDeclaration> simpleClass{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.SimpleClass"))};
        Assert::IsTrue(simpleClass->staticInterface()->fullName() == L"NativeScript.Tests.Fixtures.ISimpleClassStatic");
        Assert::IsTrue(simpleClass->staticInterface()->id() == IID{0xF07E1852, 0xC1F3, 0x5BE8,{0x5A, 0x6F, 0x6A, 0xC5, 0x36, 0xDE, 0x8E, 0xEB}});
    }
};

}
}
