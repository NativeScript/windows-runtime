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

    TEST_METHOD(SimpleClass_Methods) {
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

    TEST_METHOD(SimpleClass_Properties) {
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
};

}
}
