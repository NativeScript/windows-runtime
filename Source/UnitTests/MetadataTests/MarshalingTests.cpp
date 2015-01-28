#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(MarshalingTests){
        public :
            TEST_METHOD(InOutParameter){
                MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.MarshalingClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };
    MethodDeclaration method{ declaration->findMethodsWithName(L"InOutParameterMethod")[0] };
    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ method.parameters() };

    MethodDeclaration::ParameterIterator it{ parameters.begin() };
    const ParameterDeclaration& inParameter{ *it };
    Assert::IsTrue(inParameter.name() == L"inParameter");
    Assert::IsTrue(inParameter.fullName() == L"inParameter");
    Assert::IsTrue(inParameter.isOut() == false);

    ++it;
    const ParameterDeclaration& outParameter{ *it };
    Assert::IsTrue(outParameter.name() == L"outParameter");
    Assert::IsTrue(outParameter.fullName() == L"outParameter");
    Assert::IsTrue(outParameter.isOut() == true);

    ++it;
    Assert::IsTrue(it == parameters.end());
}

TEST_METHOD(MethodWithInt32) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.MarshalingClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };
    MethodDeclaration method{ declaration->findMethodsWithName(L"MethodWithInt32")[0] };

    PCCOR_SIGNATURE returnType{ method.returnType() };
    Assert::IsTrue(CorSigUncompressElementType(returnType) == ELEMENT_TYPE_I4);

    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ method.parameters() };
    PCCOR_SIGNATURE parameterType{(*parameters.begin()).type() };
    Assert::IsTrue(CorSigUncompressElementType(parameterType) == ELEMENT_TYPE_I4);
}
};
}
}
