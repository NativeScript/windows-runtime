#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(ClassesTests){
        public :
            TEST_METHOD(EmptyClass){
                MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EmptyClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    Assert::IsTrue(declaration->name() == L"EmptyClass");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->baseFullName() == L"System.Object");
    Assert::IsTrue(declaration->defaultInterface().fullName() == L"NativeScript.TestFixtures.IEmptyClassClass");
}

TEST_METHOD(ImplementedInterfaces) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    IteratorRange<ClassDeclaration::InterfaceIterator> interfaces{ declaration->implementedInterfaces() };
    ClassDeclaration::InterfaceIterator it{ interfaces.begin() };

    const unique_ptr<const InterfaceDeclaration>& implementedInterface1{ *it };
    Assert::IsTrue(implementedInterface1->fullName() == L"NativeScript.TestFixtures.ISimpleClassClass");

    ++it;
    const unique_ptr<const InterfaceDeclaration>& implementedInterface2{ *it };
    Assert::IsTrue(implementedInterface2->fullName() == L"Windows.Foundation.IStringable");

    ++it;
    Assert::IsTrue(it == interfaces.end());
}

TEST_METHOD(Methods) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    IteratorRange<ClassDeclaration::MethodIterator> methods{ declaration->methods() };
    ClassDeclaration::MethodIterator it{ methods.begin() };

    const MethodDeclaration& staticMethod{ *it };
    Assert::IsTrue(staticMethod.name() == L"StaticMethod");
    Assert::IsTrue(staticMethod.fullName() == L"StaticMethod");
    Assert::IsTrue(staticMethod.isStatic() == true);
    Assert::IsTrue(staticMethod.isSealed() == true);

    ++it;
    const MethodDeclaration& instanceMethod{ *it };
    Assert::IsTrue(instanceMethod.name() == L"InstanceMethod");
    Assert::IsTrue(instanceMethod.fullName() == L"InstanceMethod");
    Assert::IsTrue(instanceMethod.isStatic() == false);
    Assert::IsTrue(instanceMethod.isSealed() == true);

    ++it;
    Assert::IsTrue(it == methods.end());
}

TEST_METHOD(OverrideableMethods) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"Windows.UI.Xaml.Application" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    MethodDeclaration overridableMethod{ declaration->findMethodsWithName(L"OnActivated")[0] };
    Assert::IsTrue(overridableMethod.isSealed() == false);
}

TEST_METHOD(Properties) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    IteratorRange<ClassDeclaration::PropertyIterator> properties{ declaration->properties() };
    ClassDeclaration::PropertyIterator it{ properties.begin() };

    const PropertyDeclaration& staticProperty{ *it };
    Assert::IsTrue(staticProperty.name() == L"StaticProperty");
    Assert::IsTrue(staticProperty.fullName() == L"StaticProperty");
    Assert::IsTrue(staticProperty.getter().name() == L"get_StaticProperty");
    Assert::IsTrue(staticProperty.setter()->name() == L"put_StaticProperty");
    Assert::IsTrue(staticProperty.isStatic() == true);
    Assert::IsTrue(staticProperty.isSealed() == true);

    ++it;
    const PropertyDeclaration& staticReadonlyProperty{ *it };
    Assert::IsTrue(staticReadonlyProperty.name() == L"StaticReadonlyProperty");
    Assert::IsTrue(staticReadonlyProperty.fullName() == L"StaticReadonlyProperty");
    Assert::IsTrue(staticReadonlyProperty.getter().name() == L"get_StaticReadonlyProperty");
    Assert::IsTrue(staticReadonlyProperty.setter() == nullptr);
    Assert::IsTrue(staticReadonlyProperty.isStatic() == true);
    Assert::IsTrue(staticReadonlyProperty.isSealed() == true);

    ++it;
    const PropertyDeclaration& instanceProperty{ *it };
    Assert::IsTrue(instanceProperty.name() == L"InstanceProperty");
    Assert::IsTrue(instanceProperty.fullName() == L"InstanceProperty");
    Assert::IsTrue(instanceProperty.getter().name() == L"get_InstanceProperty");
    Assert::IsTrue(instanceProperty.setter()->name() == L"put_InstanceProperty");
    Assert::IsTrue(instanceProperty.isStatic() == false);

    ++it;
    const PropertyDeclaration& instanceReadonlyProperty{ *it };
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

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.ActivatableClass")) };

    IteratorRange<ClassDeclaration::MethodIterator> initializers{ declaration->initializers() };

    ClassDeclaration::MethodIterator it{ initializers.begin() };
    const MethodDeclaration& initializer0{ *it };
    Assert::IsTrue(initializer0.numberOfParameters() == 0);
    size_t index0{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory0{ findDeclaringInterfaceForMethod(initializer0, &index0) };
    Assert::IsTrue(initializerFactory0 == nullptr);
    Assert::IsTrue(index0 == SIZE_T_MAX);

    ++it;
    const MethodDeclaration& initializer1{ *it };
    Assert::IsTrue(initializer1.numberOfParameters() == 1);
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory1{ findDeclaringInterfaceForMethod(initializer1, &index1) };
    Assert::IsTrue(initializerFactory1->id() == IID{ 0x28D8CE19, 0x1876, 0x5679, { 0x71, 0xC8, 0x48, 0x45, 0xFC, 0xBB, 0xEB, 0x81 } });
    Assert::IsTrue(index1 == 0);

    ++it;
    const MethodDeclaration& initializer2{ *it };
    Assert::IsTrue(initializer2.numberOfParameters() == 2);
    size_t index2{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory2{ findDeclaringInterfaceForMethod(initializer2, &index2) };
    Assert::IsTrue(initializerFactory2->id() == IID{ 0x28D8CE19, 0x1876, 0x5679, { 0x71, 0xC8, 0x48, 0x45, 0xFC, 0xBB, 0xEB, 0x81 } });
    Assert::IsTrue(index2 == 1);

    ++it;
    const MethodDeclaration& initializer3{ *it };
    Assert::IsTrue(initializer3.numberOfParameters() == 3);
    size_t index3{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory3{ findDeclaringInterfaceForMethod(initializer3, &index3) };
    Assert::IsTrue(initializerFactory3->id() == IID{ 0xA71A2F94, 0xE538, 0x50EE, { 0x6C, 0x2E, 0xF8, 0x11, 0xF4, 0x69, 0x37, 0xC9 } });
    Assert::IsTrue(index3 == 0);

    ++it;
    const MethodDeclaration& initializer4{ *it };
    Assert::IsTrue(initializer4.numberOfParameters() == 4);
    size_t index4{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory4{ findDeclaringInterfaceForMethod(initializer4, &index4) };
    Assert::IsTrue(initializerFactory4->id() == IID{ 0xA71A2F94, 0xE538, 0x50EE, { 0x6C, 0x2E, 0xF8, 0x11, 0xF4, 0x69, 0x37, 0xC9 } });
    Assert::IsTrue(index4 == 1);

    ++it;
    const MethodDeclaration& initializer5{ *it };
    Assert::IsTrue(initializer5.numberOfParameters() == 5);
    size_t index5{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory5{ findDeclaringInterfaceForMethod(initializer5, &index5) };
    Assert::IsTrue(initializerFactory5->id() == IID{ 0xF4D6F8E4, 0x4979, 0x5747, { 0x4A, 0xF2, 0x73, 0xEA, 0x7F, 0x24, 0xEB, 0x8F } });
    Assert::IsTrue(index5 == 0);

    ++it;
    Assert::IsTrue(it == initializers.end());
}

TEST_METHOD(ComposablebleInitializers) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.DataTemplateKey")) };

    IteratorRange<ClassDeclaration::MethodIterator> initializers{ declaration->initializers() };

    ClassDeclaration::MethodIterator it{ initializers.begin() };
    const MethodDeclaration& initializer0{ *it };
    Assert::IsTrue(initializer0.numberOfParameters() == 0);
    size_t index0{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory0{ findDeclaringInterfaceForMethod(initializer0, &index0) };
    Assert::IsTrue(initializerFactory0->fullName() == L"Windows.UI.Xaml.IDataTemplateKeyFactory");
    Assert::IsTrue(index0 == 0);

    ++it;
    const MethodDeclaration& initializer1{ *it };
    Assert::IsTrue(initializer1.numberOfParameters() == 1);
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> initializerFactory1{ findDeclaringInterfaceForMethod(initializer1, &index1) };
    Assert::IsTrue(initializerFactory1->fullName() == L"Windows.UI.Xaml.IDataTemplateKeyFactory");
    Assert::IsTrue(index1 == 1);

    ++it;
    Assert::IsTrue(it == initializers.end());
}

TEST_METHOD(StaticMethods) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.StaticMethodsClass")) };

    MethodDeclaration complexStaticMethod{ declaration->findMethodsWithName(L"ComplexStaticMethod")[0] };
    Assert::IsTrue(complexStaticMethod.numberOfParameters() == 9);
    size_t complexStaticMethodIndex{ 0 };
    unique_ptr<const InterfaceDeclaration> complexStaticMethodIndexInterface{ findDeclaringInterfaceForMethod(complexStaticMethod, &complexStaticMethodIndex) };
    Assert::IsTrue(complexStaticMethodIndexInterface->id() == IID{ 0x74214197, 0xA903, 0x51CA, { 0x74, 0x76, 0x47, 0xFA, 0x5D, 0x93, 0xD5, 0x93 } });
    Assert::IsTrue(complexStaticMethodIndex == 0);

    vector<MethodDeclaration> methods{ declaration->findMethodsWithName(L"StaticMethod") };

    ClassDeclaration::MethodIterator it{ methods.begin() };
    const MethodDeclaration& method0{ *it };
    Assert::IsTrue(method0.numberOfParameters() == 0);
    size_t index0{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface0{ findDeclaringInterfaceForMethod(method0, &index0) };
    Assert::IsTrue(methodInterface0->id() == IID{ 0x8DA4180D, 0xBB47, 0x5AC8, { 0x41, 0xFA, 0x5C, 0xA1, 0xF6, 0xB3, 0xB8, 0x33 } });
    Assert::IsTrue(index0 == 0);

    ++it;
    const MethodDeclaration& method1{ *it };
    Assert::IsTrue(method1.numberOfParameters() == 1);
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface1{ findDeclaringInterfaceForMethod(method1, &index1) };
    Assert::IsTrue(methodInterface1->id() == IID{ 0x8DA4180D, 0xBB47, 0x5AC8, { 0x41, 0xFA, 0x5C, 0xA1, 0xF6, 0xB3, 0xB8, 0x33 } });
    Assert::IsTrue(index1 == 1);

    ++it;
    const MethodDeclaration& method2{ *it };
    Assert::IsTrue(method2.numberOfParameters() == 2);
    size_t index2{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface2{ findDeclaringInterfaceForMethod(method2, &index2) };
    Assert::IsTrue(methodInterface2->id() == IID{ 0x207FAAFF, 0x68B5, 0x5CC8, { 0x5B, 0xA8, 0xE7, 0x0C, 0xD9, 0x47, 0xD3, 0x07 } });
    Assert::IsTrue(index2 == 0);

    ++it;
    const MethodDeclaration& method3{ *it };
    Assert::IsTrue(method3.numberOfParameters() == 3);
    size_t index3{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface3{ findDeclaringInterfaceForMethod(method3, &index3) };
    Assert::IsTrue(methodInterface3->id() == IID{ 0x207FAAFF, 0x68B5, 0x5CC8, { 0x5B, 0xA8, 0xE7, 0x0C, 0xD9, 0x47, 0xD3, 0x07 } });
    Assert::IsTrue(index3 == 1);

    ++it;
    Assert::IsTrue(it == methods.end());
}

TEST_METHOD(InstanceMethods) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.InstanceMethodsClass")) };

    PropertyDeclaration property{ *static_cast<const PropertyDeclaration*>(declaration->findMembersWithName(L"Address")[0].get()) };
    size_t setterIndex{ 0 };
    unique_ptr<const InterfaceDeclaration> toStringInterface{ findDeclaringInterfaceForMethod(*property.setter(), &setterIndex) };
    Assert::IsTrue(toStringInterface->fullName() == L"Windows.ApplicationModel.Appointments.IAppointmentParticipant");
    Assert::IsTrue(setterIndex == 3);

    vector<MethodDeclaration> methods{ declaration->findMethodsWithName(L"InstanceMethod") };

    ClassDeclaration::MethodIterator it{ methods.begin() };
    const MethodDeclaration& method0{ *it };
    Assert::IsTrue(method0.numberOfParameters() == 0);
    size_t index0{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface0{ findDeclaringInterfaceForMethod(method0, &index0) };
    Assert::IsTrue(methodInterface0->id() == IID{ 0xA47B5AFB, 0xE1C0, 0x5A73, { 0x5F, 0xEA, 0xDE, 0xC5, 0x03, 0xBE, 0x33, 0x9E } });
    Assert::IsTrue(index0 == 0);

    ++it;
    const MethodDeclaration& method1{ *it };
    Assert::IsTrue(method1.numberOfParameters() == 1);
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface1{ findDeclaringInterfaceForMethod(method1, &index1) };
    Assert::IsTrue(methodInterface1->id() == IID{ 0xA47B5AFB, 0xE1C0, 0x5A73, { 0x5F, 0xEA, 0xDE, 0xC5, 0x03, 0xBE, 0x33, 0x9E } });
    Assert::IsTrue(index1 == 1);

    ++it;
    const MethodDeclaration& method2{ *it };
    Assert::IsTrue(method2.numberOfParameters() == 2);
    size_t index2{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface2{ findDeclaringInterfaceForMethod(method2, &index2) };
    Assert::IsTrue(methodInterface2->id() == IID{ 0xB499E488, 0x7448, 0x589B, { 0x4B, 0x43, 0x1D, 0x5F, 0x12, 0x2B, 0x4E, 0x75 } });
    Assert::IsTrue(index2 == 0);

    ++it;
    const MethodDeclaration& method3{ *it };
    Assert::IsTrue(method3.numberOfParameters() == 3);
    size_t index3{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface3{ findDeclaringInterfaceForMethod(method3, &index3) };
    Assert::IsTrue(methodInterface3->id() == IID{ 0xB499E488, 0x7448, 0x589B, { 0x4B, 0x43, 0x1D, 0x5F, 0x12, 0x2B, 0x4E, 0x75 } });
    Assert::IsTrue(index3 == 1);

    ++it;
    Assert::IsTrue(it == methods.end());
}

TEST_METHOD(GenericMethodImplementation1) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.MultiGenericImplementationClass")) };

    MethodDeclaration method1{ declaration->findMethodsWithName(L"Windows.Foundation.Collections.IIterable`1<Int16>.First")[0] };
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface1{ findDeclaringInterfaceForMethod(method1, &index1) };
    Assert::IsTrue(methodInterface1->fullName() == L"Windows.Foundation.Collections.IIterable`1<Int16>");
    Assert::IsTrue(methodInterface1->id() == IID{ 0x72FF2923, 0x4B4E, 0x53BB, { 0x8F, 0xEB, 0x41, 0xEC, 0x5F, 0x2B, 0xB7, 0x34 } });
    Assert::IsTrue(index1 == 0);

    MethodDeclaration method2{ declaration->findMethodsWithName(L"Windows.Foundation.Collections.IIterable`1<Int32>.First")[0] };
    size_t index2{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface2{ findDeclaringInterfaceForMethod(method2, &index2) };
    Assert::IsTrue(methodInterface2->id() == IID{ 0x81a643fb, 0xf51c, 0x5565, { 0x83, 0xc4, 0xf9, 0x64, 0x25, 0x77, 0x7b, 0x66 } });
    Assert::IsTrue(index2 == 0);
}

TEST_METHOD(GenericMethodImplementation2) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.Data.Json.JsonArray")) };

    MethodDeclaration method1{ declaration->findMethodsWithName(L"First")[0] };
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface1{ findDeclaringInterfaceForMethod(method1, &index1) };
    Assert::IsTrue(methodInterface1->fullName() == L"Windows.Foundation.Collections.IIterable`1<Windows.Data.Json.IJsonValue>");
    Assert::IsTrue(methodInterface1->id() == IID{ 0xCB0492B6, 0x4113, 0x55CF, { 0xB2, 0xC5, 0x99, 0xEB, 0x42, 0x8B, 0xA4, 0x93 } });
    Assert::IsTrue(index1 == 0);
}

TEST_METHOD(ExplicitInterfaceMethodImplementation) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.ExplicitInterfaceMethodImplementationClass")) };

    MethodDeclaration method0{ declaration->findMethodsWithName(L"Method")[0] };
    Assert::IsTrue(method0.fullName() == L"Method");
    size_t index0{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface0{ findDeclaringInterfaceForMethod(method0, &index0) };
    Assert::IsTrue(methodInterface0->id() == IID{ 0x55E51692, 0x0CE8, 0x5A02, { 0x42, 0xF1, 0x78, 0x4B, 0xB7, 0x2D, 0x63, 0x41 } });
    Assert::IsTrue(index0 == 0);

    MethodDeclaration method1{ declaration->findMethodsWithName(L"NativeScript.TestFixtures.IExplicitInterface1.Method")[0] };
    size_t index1{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface1{ findDeclaringInterfaceForMethod(method1, &index1) };
    Assert::IsTrue(methodInterface1->fullName() == L"NativeScript.TestFixtures.IExplicitInterface1");
    Assert::IsTrue(index1 == 0);

    MethodDeclaration method2{ declaration->findMethodsWithName(L"NativeScript.TestFixtures.IExplicitInterface2.Method")[0] };
    size_t index2{ 0 };
    unique_ptr<const InterfaceDeclaration> methodInterface2{ findDeclaringInterfaceForMethod(method2, &index2) };
    Assert::IsTrue(methodInterface2->fullName() == L"NativeScript.TestFixtures.IExplicitInterface2");
    Assert::IsTrue(index2 == 0);
}

TEST_METHOD(ClassType) {
    MetadataReader metadataReader;

    shared_ptr<ClassDeclaration> emptyClass{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.EmptyClass")) };
    Assert::IsTrue(emptyClass->isInstantiable() == false);
    Assert::IsTrue(emptyClass->isSealed() == true);

    shared_ptr<ClassDeclaration> activatableClass{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"NativeScript.TestFixtures.ActivatableClass")) };
    Assert::IsTrue(activatableClass->isInstantiable() == true);
    Assert::IsTrue(activatableClass->isSealed() == true);

    shared_ptr<ClassDeclaration> composableClass{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.Controls.ListView")) };
    Assert::IsTrue(composableClass->isInstantiable() == true);
    Assert::IsTrue(composableClass->isSealed() == false);

    shared_ptr<ClassDeclaration> abstractClass{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(L"Windows.UI.Xaml.Controls.Control")) };
    Assert::IsTrue(abstractClass->isInstantiable() == false);
    Assert::IsTrue(abstractClass->isSealed() == false);
}
};
}
}
