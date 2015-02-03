#include "UnitTests-Prefix.h"
#include <algorithm>

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(InterfacesTests){
        public :
            TEST_METHOD(EmptyInterface){
                const wchar_t * name{ L"NativeScript.TestFixtures.IEmptyInterface" };
    const InterfaceDeclaration* declaration{ static_cast<const InterfaceDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->name() == L"IEmptyInterface");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->id() == IID{ 0xB90BDBA0, 0x89D5, 0x5502, { 0x42, 0x23, 0xB6, 0x7B, 0xF5, 0x2C, 0x7C, 0x3B } });
}

TEST_METHOD(OpenGenericInterface) {
    const wchar_t* name{ L"Windows.Foundation.Collections.IIterable`1" };
    const GenericInterfaceDeclaration* declaration{ static_cast<const GenericInterfaceDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->name() == L"IIterable");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->numberOfGenericParameters() == 1);
}

TEST_METHOD(ClosedGenericInterface) {
    const ClassDeclaration* declaration{ static_cast<const ClassDeclaration*>(MetadataReader::findByName(L"NativeScript.TestFixtures.MultiGenericImplementationClass")) };

    // TODO
    MultiGenericImplementationClass ^ instance(ref new MultiGenericImplementationClass());
    for (const unique_ptr<const InterfaceDeclaration>& implementedInterface : declaration->implementedInterfaces()) {
        wstring name{ implementedInterface->fullName() };
        Assert::IsTrue(name.data());

        IID id = implementedInterface->id();
        Microsoft::WRL::ComPtr<IUnknown> instanceUnknown;
        Assert::IsTrue(reinterpret_cast<IInspectable*>(instance)->QueryInterface(id, &instanceUnknown) == S_OK);
    }
}

TEST_METHOD(ClosedGenericInterfaceImplementation) {
    const wchar_t* name{ L"NativeScript.TestFixtures.SingleGenericImplementationClass" };
    const ClassDeclaration* declaration{ static_cast<const ClassDeclaration*>(MetadataReader::findByName(name)) };

    IteratorRange<ClassDeclaration::InterfaceIterator> interfaces{ declaration->implementedInterfaces() };

    ClassDeclaration::InterfaceIterator it{ find_if(interfaces.begin(), interfaces.end(), [](const unique_ptr<const InterfaceDeclaration>& i) {
            return i->fullName() == L"Windows.Foundation.Collections.IIterable`1<Int32>";
    }) };
    Assert::IsTrue(it != interfaces.end());
}
};
}
}
