#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(MarshalingTests) {
public:
    TEST_METHOD(InOutParameter) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.MarshalingClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};
        shared_ptr<MethodDeclaration> method{dynamic_pointer_cast<MethodDeclaration>(declaration->findMembersWithName(L"InOutParameterMethod")[0])};
        IteratorRange<MethodDeclaration::ParameterIterator> parameters{method->parameters()};

        MethodDeclaration::ParameterIterator it{parameters.begin()};
        ParameterDeclaration inParameter{*it};
        Assert::IsTrue(inParameter.name() == L"inParameter");
        Assert::IsTrue(inParameter.fullName() == L"NativeScript.Tests.Fixtures.MarshalingClass.InOutParameterMethod.inParameter");
        Assert::IsTrue(inParameter.isIn());
        Assert::IsTrue(!inParameter.isOut());

        ++it;
        ParameterDeclaration outParameter{*it};
        Assert::IsTrue(outParameter.name() == L"outParameter");
        Assert::IsTrue(outParameter.fullName() == L"NativeScript.Tests.Fixtures.MarshalingClass.InOutParameterMethod.outParameter");
        Assert::IsTrue(!outParameter.isIn());
        Assert::IsTrue(outParameter.isOut());

        ++it;
        Assert::IsTrue(it == parameters.end());
    }
};

}
}
