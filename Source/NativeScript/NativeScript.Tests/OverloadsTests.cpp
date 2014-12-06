#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(OverloadsTests) {
public:
    TEST_METHOD(MethodOverloads) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.MethodOverloads"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};
        vector<shared_ptr<Declaration>> methods{declaration->findMembersWithName(L"Method")};

        Assert::IsTrue(methods.size() == 2);

        shared_ptr<MethodDeclaration> method1{dynamic_pointer_cast<MethodDeclaration>(methods[0])};
        Assert::IsTrue(method1->name() == L"Method");
        Assert::IsTrue(method1->overloadName() == L"MethodWithDouble");
        Assert::IsTrue(method1->isDefaultOverload() == true);

        shared_ptr<MethodDeclaration> method2{dynamic_pointer_cast<MethodDeclaration>(methods[1])};
        Assert::IsTrue(method2->name() == L"Method");
        Assert::IsTrue(method2->overloadName() == L"MethodWithInt");
        Assert::IsTrue(method2->isDefaultOverload() == false);

        shared_ptr<MethodDeclaration> methodWithDouble{dynamic_pointer_cast<MethodDeclaration>(declaration->findMembersWithName(L"MethodWithDouble")[0])};
        Assert::IsTrue(methodWithDouble->name() == L"MethodWithDouble");
        Assert::IsTrue(methodWithDouble->overloadName() == L"");
        Assert::IsTrue(methodWithDouble->isDefaultOverload() == false);

        shared_ptr<MethodDeclaration> methodWithInt{dynamic_pointer_cast<MethodDeclaration>(declaration->findMembersWithName(L"MethodWithInt")[0])};
        Assert::IsTrue(methodWithInt->name() == L"MethodWithInt");
        Assert::IsTrue(methodWithInt->overloadName() == L"");
        Assert::IsTrue(methodWithInt->isDefaultOverload() == false);
    }
};

}
}
