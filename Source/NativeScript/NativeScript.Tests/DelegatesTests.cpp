#include "pch.h"
#include <iomanip>

namespace NativeScript {
namespace Tests {

TEST_CLASS(DelegatesTests) {
public:
    TEST_METHOD(EmptyDelegate) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EmptyDelegate"};
        shared_ptr<DelegateDeclaration> declaration{dynamic_pointer_cast<DelegateDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"EmptyDelegate");
        Assert::IsTrue(declaration->fullName() == name);
        Assert::IsTrue(declaration->id() == CLSID{0xAA85FC70, 0x23F8, 0x510B,{0x50, 0x4D, 0xFB, 0x8E, 0xAB, 0x16, 0x93, 0x91}});
        Assert::IsTrue(declaration->numberOfParameters() == 0);
        Assert::IsTrue(declaration->parameters().begin() == declaration->parameters().end());
    }

    TEST_METHOD(SimpleDelegate) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleDelegate"};
        shared_ptr<DelegateDeclaration> declaration{dynamic_pointer_cast<DelegateDeclaration>(metadataReader.findByName(name))};
        Assert::IsTrue(declaration->numberOfParameters() == 2);

        IteratorRange<DelegateDeclaration::ParameterIterator> parameters{declaration->parameters()};

        DelegateDeclaration::ParameterIterator it{parameters.begin()};
        ParameterDeclaration inParameter = *it;
        Assert::IsTrue(inParameter.name() == L"inParameter");
        Assert::IsTrue(inParameter.isIn() == true);
        Assert::IsTrue(inParameter.isOut() == false);

        ++it;
        ParameterDeclaration outParameter = *it;
        Assert::IsTrue(outParameter.name() == L"outParameter");
        Assert::IsTrue(outParameter.isIn() == false);
        Assert::IsTrue(outParameter.isOut() == true);

        ++it;
        Assert::IsTrue(it == parameters.end());
    }
};

}
}
