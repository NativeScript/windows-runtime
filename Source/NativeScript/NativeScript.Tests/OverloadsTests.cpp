#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(OverloadsTests) {
public:
    TEST_METHOD(MethodOverloads) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.OverloadsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};
        vector<shared_ptr<MethodDeclaration>> methods{declaration->findMethodsWithName(L"InstanceMethod")};

        Assert::IsTrue(methods.size() == 2);

        shared_ptr<MethodDeclaration> method1{methods[0]};
        Assert::IsTrue(method1->name() == L"InstanceMethod");
        Assert::IsTrue(method1->overloadName() == L"InstanceMethodWithDouble");
        Assert::IsTrue(method1->isDefaultOverload() == true);

        shared_ptr<MethodDeclaration> method2{methods[1]};
        Assert::IsTrue(method2->name() == L"InstanceMethod");
        Assert::IsTrue(method2->overloadName() == L"InstanceMethodWithInt");
        Assert::IsTrue(method2->isDefaultOverload() == false);

        shared_ptr<MethodDeclaration> methodWithDouble{declaration->findMethodsWithName(L"InstanceMethodWithDouble")[0]};
        Assert::IsTrue(methodWithDouble->name() == L"InstanceMethodWithDouble");
        Assert::IsTrue(methodWithDouble->overloadName().empty());
        Assert::IsTrue(methodWithDouble->isDefaultOverload() == false);

        shared_ptr<MethodDeclaration> methodWithInt{declaration->findMethodsWithName(L"InstanceMethodWithInt")[0]};
        Assert::IsTrue(methodWithInt->name() == L"InstanceMethodWithInt");
        Assert::IsTrue(methodWithInt->overloadName().empty());
        Assert::IsTrue(methodWithInt->isDefaultOverload() == false);
    }
};

}
}
