#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(OverloadsTests) {
public:
    TEST_METHOD(MethodOverloads) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.OverloadsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};
        vector<const MethodDeclaration> methods{declaration->findMethodsWithName(L"InstanceMethod")};

        Assert::IsTrue(methods.size() == 2);

        const MethodDeclaration& method1{methods[0]};
        Assert::IsTrue(method1.name() == L"InstanceMethod");
        Assert::IsTrue(method1.overloadName() == L"InstanceMethod");
        Assert::IsTrue(method1.isDefaultOverload() == true);

        const MethodDeclaration& method2{methods[1]};
        Assert::IsTrue(method2.name() == L"InstanceMethod");
        Assert::IsTrue(method2.overloadName() == L"InstanceMethodWithInt");
        Assert::IsTrue(method2.isDefaultOverload() == false);
    }
};

}
}
