#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(DelegatesTests){
        public :
            TEST_METHOD(EmptyDelegate){
                MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EmptyDelegate" };
    shared_ptr<DelegateDeclaration> declaration{ static_pointer_cast<DelegateDeclaration>(metadataReader.findByName(name)) };

    Assert::IsTrue(declaration->name() == L"EmptyDelegate");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->id() == CLSID{ 0x69F5CF98, 0x0489, 0x5DAF, { 0x63, 0x79, 0xE5, 0xFB, 0x42, 0x66, 0x7C, 0x5B } });
    Assert::IsTrue(declaration->invokeMethod().parameters().begin() == declaration->invokeMethod().parameters().end());
}

TEST_METHOD(SimpleDelegate) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleDelegate" };
    shared_ptr<DelegateDeclaration> declaration{ static_pointer_cast<DelegateDeclaration>(metadataReader.findByName(name)) };

    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ declaration->invokeMethod().parameters() };

    MethodDeclaration::ParameterIterator it{ parameters.begin() };
    const ParameterDeclaration& inParameter{ *it };
    Assert::IsTrue(inParameter.name() == L"inParameter");
    Assert::IsTrue(inParameter.isOut() == false);

    ++it;
    const ParameterDeclaration& outParameter{ *it };
    Assert::IsTrue(outParameter.name() == L"outParameter");
    Assert::IsTrue(outParameter.isOut() == true);

    ++it;
    Assert::IsTrue(it == parameters.end());
}

TEST_METHOD(GenericDelegate) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"Windows.Foundation.EventHandler`1" };
    shared_ptr<GenericDelegateDeclaration> declaration{ static_pointer_cast<GenericDelegateDeclaration>(metadataReader.findByName(name)) };

    Assert::IsTrue(declaration->name() == L"EventHandler");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->numberOfGenericParameters() == 1);

    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ declaration->invokeMethod().parameters() };

    MethodDeclaration::ParameterIterator it{ parameters.begin() };
    const ParameterDeclaration& inParameter{ *it };
    Assert::IsTrue(inParameter.name() == L"sender");

    ++it;
    const ParameterDeclaration& outParameter{ *it };
    Assert::IsTrue(outParameter.name() == L"args");

    ++it;
    Assert::IsTrue(it == parameters.end());
}
};
}
}
