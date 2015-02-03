#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(NamespacesTests){
        public :
            TEST_METHOD(Namespaces){
                initializer_list<pair<const wchar_t*, const wchar_t*>> cases{
                    pair<const wchar_t*, const wchar_t*>(L"", L""),
                    pair<const wchar_t*, const wchar_t*>(L"NativeScript", L"NativeScript"),
                    pair<const wchar_t*, const wchar_t*>(L"NativeScript.TestFixtures", L"TestFixtures"),
                    pair<const wchar_t*, const wchar_t*>(L"NativeScript.TestFixtures.InnerNamespace1", L"InnerNamespace1"),
                    pair<const wchar_t*, const wchar_t*>(L"NativeScript.TestFixtures.InnerNamespace1.InnerNamespace2", L"InnerNamespace2"),
                };

    for (pair<const wchar_t*, const wchar_t*> pair : cases) {
        const NamespaceDeclaration* declaration{ static_cast<const NamespaceDeclaration*>(MetadataReader::findByName(pair.first)) };
        Assert::IsTrue(declaration->name() == pair.second);
        Assert::IsTrue(declaration->fullName() == pair.first);
    }
}

TEST_METHOD(WindowsNamespaces) {
    initializer_list<pair<const wchar_t*, const wchar_t*>> cases{
        pair<const wchar_t*, const wchar_t*>(L"Windows", L"Windows"),
        pair<const wchar_t*, const wchar_t*>(L"Windows.Foundation", L"Foundation"),
        pair<const wchar_t*, const wchar_t*>(L"Windows.Foundation.Collections", L"Collections"),
    };

    for (pair<const wchar_t*, const wchar_t*> pair : cases) {
        const NamespaceDeclaration* declaration{ static_cast<const NamespaceDeclaration*>(MetadataReader::findByName(pair.first)) };
        Assert::IsTrue(declaration->name() == pair.second);
        Assert::IsTrue(declaration->fullName() == pair.first);
    }
}

TEST_METHOD(InvalidNamespaces) {
    Assert::IsTrue(MetadataReader::findByName(L"Windows.") == nullptr);
    Assert::IsTrue(MetadataReader::findByName(L".Windows") == nullptr);
}
};
}
}
