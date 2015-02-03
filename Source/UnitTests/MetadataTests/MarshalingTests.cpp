#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(MarshalingTests){
        public :
            TEST_METHOD(InOutParameter){
                const wchar_t * name{ L"NativeScript.TestFixtures.MarshalingClass" };
    const ClassDeclaration* declaration{ static_cast<const ClassDeclaration*>(MetadataReader::findByName(name)) };
    MethodDeclaration method{ declaration->findMethodsWithName(L"InOutParameterMethod")[0] };
    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ method.parameters() };

    MethodDeclaration::ParameterIterator it{ parameters.begin() };
    const ParameterDeclaration& inParameter{ *it };
    Assert::IsTrue(inParameter.name() == L"inParameter");
    Assert::IsTrue(inParameter.fullName() == L"inParameter");
    // TODO
    //    Assert::IsTrue(inParameter.isOut() == false);

    ++it;
    const ParameterDeclaration& outParameter{ *it };
    Assert::IsTrue(outParameter.name() == L"outParameter");
    Assert::IsTrue(outParameter.fullName() == L"outParameter");
    //    Assert::IsTrue(outParameter.isOut() == true);

    ++it;
    Assert::IsTrue(it == parameters.end());
}

TEST_METHOD(MethodWithInt32) {
    const wchar_t* name{ L"NativeScript.TestFixtures.MarshalingClass" };
    const ClassDeclaration* declaration{ static_cast<const ClassDeclaration*>(MetadataReader::findByName(name)) };
    MethodDeclaration method{ declaration->findMethodsWithName(L"MethodWithInt32")[0] };

    Assert::IsTrue(method.returnType().elementType() == ElementType::Int32);

    IteratorRange<MethodDeclaration::ParameterIterator> parameters{ method.parameters() };
    Assert::IsTrue((*parameters.begin()).type().elementType() == ElementType::Int32);
}
};
}
}
