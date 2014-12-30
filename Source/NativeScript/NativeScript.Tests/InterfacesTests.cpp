#include "pch.h"
#include <algorithm>

namespace NativeScript {
namespace Tests {

TEST_CLASS(InterfacesTests) {
public:
    TEST_METHOD(EmptyInterface) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.IEmptyInterface"};
        shared_ptr<InterfaceDeclaration> declaration{dynamic_pointer_cast<InterfaceDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"IEmptyInterface");
        Assert::IsTrue(declaration->fullName() == name);
        Assert::IsTrue(declaration->id() == IID{0x0B309244, 0x79B4, 0x50BE,{0x42, 0xAD, 0xD0, 0xE5, 0xFE, 0x4e, 0x78, 0x43}});
    }

    TEST_METHOD(OpenGenericInterface) {
        MetadataReader metadataReader;

        const wchar_t* name{L"Windows.Foundation.Collections.IIterable`1"};
        shared_ptr<InterfaceDeclaration> declaration{dynamic_pointer_cast<InterfaceDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"IIterable");
        Assert::IsTrue(declaration->fullName() == name);
    }

    TEST_METHOD(ClosedGenericInterfaceImplementation) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SingleGenericImplementationClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        IteratorRange<ClassDeclaration::InterfaceIterator> interfaces{declaration->implementedInterfaces()};

        ClassDeclaration::InterfaceIterator it{find_if(interfaces.begin(), interfaces.end(), [](const unique_ptr<const InterfaceDeclaration>& i) {
            return i->fullName() == L"Windows.Foundation.Collections.IIterable`1";
        })};

        Assert::IsTrue(it != interfaces.end());
    }
};

}
}
