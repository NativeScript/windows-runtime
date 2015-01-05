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

    TEST_METHOD(ActivatableInitializers) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.ActivatableClass"))};

        IteratorRange<ClassDeclaration::MethodIterator> initializers{declaration->initializers()};

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

    TEST_METHOD(ComposablebleInitializers) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.Controls.Maps.LocalMapTileDataSource"))};

        IteratorRange<ClassDeclaration::MethodIterator> initializers{declaration->initializers()};

        ClassDeclaration::MethodIterator it{initializers.begin()};
        const MethodDeclaration& initializer0{*it};
        Assert::IsTrue(initializer0.numberOfParameters() == 0);
        size_t index0{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory0{findDeclaringInterfaceForMethod(initializer0, &index0)};
        Assert::IsTrue(initializerFactory0->fullName() == L"Windows.UI.Xaml.Controls.Maps.ILocalMapTileDataSourceFactory");
        Assert::IsTrue(index0 == 0);

        ++it;
        const MethodDeclaration& initializer1{*it};
        Assert::IsTrue(initializer1.numberOfParameters() == 1);
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> initializerFactory1{findDeclaringInterfaceForMethod(initializer1, &index1)};
        Assert::IsTrue(initializerFactory1->fullName() == L"Windows.UI.Xaml.Controls.Maps.ILocalMapTileDataSourceFactory");
        Assert::IsTrue(index1 == 1);

        ++it;
        Assert::IsTrue(it == initializers.end());
    }

    TEST_METHOD(StaticMethods) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.StaticMethodsClass"))};

        MethodDeclaration complexStaticMethod{declaration->findMethodsWithName(L"ComplexStaticMethod")[0]};
        Assert::IsTrue(complexStaticMethod.numberOfParameters() == 9);
        size_t complexStaticMethodIndex{0};
        unique_ptr<const InterfaceDeclaration> complexStaticMethodIndexInterface{findDeclaringInterfaceForMethod(complexStaticMethod, &complexStaticMethodIndex)};
        Assert::IsTrue(complexStaticMethodIndexInterface->id() == IID{0x05209319, 0xECCD, 0x5E67,{0x4D, 0xF4, 0x75, 0xE5, 0x19, 0xEE, 0xE5, 0xC3}});
        Assert::IsTrue(complexStaticMethodIndex == 0);

        vector<const MethodDeclaration> methods{declaration->findMethodsWithName(L"StaticMethod")};

        ClassDeclaration::MethodIterator it{methods.begin()};
        const MethodDeclaration& method0{*it};
        Assert::IsTrue(method0.numberOfParameters() == 0);
        size_t index0{0};
        unique_ptr<const InterfaceDeclaration> methodInterface0{findDeclaringInterfaceForMethod(method0, &index0)};
        Assert::IsTrue(methodInterface0->id() == IID{0xAF5D09E3, 0x5FD6, 0x5D13,{0x71, 0x49, 0xEA, 0x97, 0x11, 0x36, 0xE9, 0x12}});
        Assert::IsTrue(index0 == 0);

        ++it;
        const MethodDeclaration& method1{*it};
        Assert::IsTrue(method1.numberOfParameters() == 1);
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> methodInterface1{findDeclaringInterfaceForMethod(method1, &index1)};
        Assert::IsTrue(methodInterface1->id() == IID{0xAF5D09E3, 0x5FD6, 0x5D13,{0x71, 0x49, 0xEA, 0x97, 0x11, 0x36, 0xE9, 0x12}});
        Assert::IsTrue(index1 == 1);

        ++it;
        const MethodDeclaration& method2{*it};
        Assert::IsTrue(method2.numberOfParameters() == 2);
        size_t index2{0};
        unique_ptr<const InterfaceDeclaration> methodInterface2{findDeclaringInterfaceForMethod(method2, &index2)};
        Assert::IsTrue(methodInterface2->id() == IID{0x1BEA6667, 0x4A41, 0x539C,{0x57, 0xE7, 0xB8, 0x59, 0xA3, 0x13, 0xD5, 0x84}});
        Assert::IsTrue(index2 == 0);

        ++it;
        const MethodDeclaration& method3{*it};
        Assert::IsTrue(method3.numberOfParameters() == 3);
        size_t index3{0};
        unique_ptr<const InterfaceDeclaration> methodInterface3{findDeclaringInterfaceForMethod(method3, &index3)};
        Assert::IsTrue(methodInterface3->id() == IID{0x1BEA6667, 0x4A41, 0x539C,{0x57, 0xE7, 0xB8, 0x59, 0xA3, 0x13, 0xD5, 0x84}});
        Assert::IsTrue(index3 == 1);

        ++it;
        Assert::IsTrue(it == methods.end());
    }

    TEST_METHOD(InstanceMethods) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.InstanceMethodsClass"))};

        PropertyDeclaration property{*dynamic_cast<const PropertyDeclaration*>(declaration->findMembersWithName(L"Address")[0].get())};
        size_t setterIndex{0};
        unique_ptr<const InterfaceDeclaration> toStringInterface{findDeclaringInterfaceForMethod(*property.setter(), &setterIndex)};
        Assert::IsTrue(toStringInterface->fullName() == L"Windows.ApplicationModel.Appointments.IAppointmentParticipant");
        Assert::IsTrue(setterIndex == 3);

        vector<const MethodDeclaration> methods{declaration->findMethodsWithName(L"InstanceMethod")};

        ClassDeclaration::MethodIterator it{methods.begin()};
        const MethodDeclaration& method0{*it};
        Assert::IsTrue(method0.numberOfParameters() == 0);
        size_t index0{0};
        unique_ptr<const InterfaceDeclaration> methodInterface0{findDeclaringInterfaceForMethod(method0, &index0)};
        Assert::IsTrue(methodInterface0->id() == IID{0x63C1D01E, 0x7401, 0x5109,{0x75, 0x73, 0x9A, 0xB3, 0x57, 0x1F, 0x0E, 0x9A}});
        Assert::IsTrue(index0 == 0);

        ++it;
        const MethodDeclaration& method1{*it};
        Assert::IsTrue(method1.numberOfParameters() == 1);
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> methodInterface1{findDeclaringInterfaceForMethod(method1, &index1)};
        Assert::IsTrue(methodInterface1->id() == IID{0x63C1D01E, 0x7401, 0x5109,{0x75, 0x73, 0x9A, 0xB3, 0x57, 0x1F, 0x0E, 0x9A}});
        Assert::IsTrue(index1 == 1);

        ++it;
        const MethodDeclaration& method2{*it};
        Assert::IsTrue(method2.numberOfParameters() == 2);
        size_t index2{0};
        unique_ptr<const InterfaceDeclaration> methodInterface2{findDeclaringInterfaceForMethod(method2, &index2)};
        Assert::IsTrue(methodInterface2->id() == IID{0xA5E2A612, 0xEF81, 0x5DBE,{0x4D, 0x3E, 0xE7, 0xE3, 0x2D, 0x66, 0xB4, 0xA5}});
        Assert::IsTrue(index2 == 0);

        ++it;
        const MethodDeclaration& method3{*it};
        Assert::IsTrue(method3.numberOfParameters() == 3);
        size_t index3{0};
        unique_ptr<const InterfaceDeclaration> methodInterface3{findDeclaringInterfaceForMethod(method3, &index3)};
        Assert::IsTrue(methodInterface3->id() == IID{0xA5E2A612, 0xEF81, 0x5DBE,{0x4D, 0x3E, 0xE7, 0xE3, 0x2D, 0x66, 0xB4, 0xA5}});
        Assert::IsTrue(index3 == 1);

        ++it;
        Assert::IsTrue(it == methods.end());
    }

    TEST_METHOD(GenericMethodImplementation1) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.DoubleGenericImplementationClass"))};

        MethodDeclaration method1{declaration->findMethodsWithName(L"Windows.Foundation.Collections.IIterable`1<Int16>.First")[0]};
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> methodInterface1{findDeclaringInterfaceForMethod(method1, &index1)};
        Assert::IsTrue(methodInterface1->fullName() == L"Windows.Foundation.Collections.IIterable`1");
        Assert::IsTrue(index1 == 0);

        MethodDeclaration method2{declaration->findMethodsWithName(L"Windows.Foundation.Collections.IIterable`1<Int32>.First")[0]};
        size_t index2{0};
        unique_ptr<const InterfaceDeclaration> methodInterface2{findDeclaringInterfaceForMethod(method2, &index2)};
        Assert::IsTrue(methodInterface2->fullName() == L"Windows.Foundation.Collections.IIterable`1");
        Assert::IsTrue(index2 == 0);
    }

    TEST_METHOD(GenericMethodImplementation2) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.Data.Json.JsonArray"))};

        MethodDeclaration method1{declaration->findMethodsWithName(L"First")[0]};
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> methodInterface1{findDeclaringInterfaceForMethod(method1, &index1)};
        Assert::IsTrue(methodInterface1->fullName() == L"Windows.Foundation.Collections.IIterable`1");
        Assert::IsTrue(index1 == 0);
    }

    TEST_METHOD(ExplicitInterfaceMethodImplementation) {
        MetadataReader metadataReader;

        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.Tests.Fixtures.ExplicitInterfaceMethodImplementationClass"))};

        MethodDeclaration method0{declaration->findMethodsWithName(L"Method")[0]};
        Assert::IsTrue(method0.fullName() == L"Method");
        size_t index0{0};
        unique_ptr<const InterfaceDeclaration> methodInterface0{findDeclaringInterfaceForMethod(method0, &index0)};
        Assert::IsTrue(methodInterface0->id() == IID{0xBAD5EA8B, 0xE896, 0x5FC0,{0x5B, 0x00, 0xB7, 0x47, 0x9E, 0x68, 0x58, 0x34}});
        Assert::IsTrue(index0 == 0);

        MethodDeclaration method1{declaration->findMethodsWithName(L"NativeScript.Tests.Fixtures.IExplicitInterface1.Method")[0]};
        size_t index1{0};
        unique_ptr<const InterfaceDeclaration> methodInterface1{findDeclaringInterfaceForMethod(method1, &index1)};
        Assert::IsTrue(methodInterface1->fullName() == L"NativeScript.Tests.Fixtures.IExplicitInterface1");
        Assert::IsTrue(index1 == 0);

        MethodDeclaration method2{declaration->findMethodsWithName(L"NativeScript.Tests.Fixtures.IExplicitInterface2.Method")[0]};
        size_t index2{0};
        unique_ptr<const InterfaceDeclaration> methodInterface2{findDeclaringInterfaceForMethod(method2, &index2)};
        Assert::IsTrue(methodInterface2->fullName() == L"NativeScript.Tests.Fixtures.IExplicitInterface2");
        Assert::IsTrue(index2 == 0);
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
};

}
}
