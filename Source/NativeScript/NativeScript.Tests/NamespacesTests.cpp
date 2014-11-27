#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(NamespacesTests) {
public:
    TEST_METHOD(Namespaces) {
        MetadataReader metadataReader;

        auto cases = {
            pair<const wchar_t*, const wchar_t*>(L"", L""),
            pair<const wchar_t*, const wchar_t*>(L"NativeScript", L"NativeScript"),
            pair<const wchar_t*, const wchar_t*>(L"NativeScript.Tests", L"Tests"),
            pair<const wchar_t*, const wchar_t*>(L"NativeScript.Tests.Fixtures", L"Fixtures"),
            pair<const wchar_t*, const wchar_t*>(L"NativeScript.Tests.Fixtures.InnerNamespace1", L"InnerNamespace1"),
            pair<const wchar_t*, const wchar_t*>(L"NativeScript.Tests.Fixtures.InnerNamespace1.InnerNamespace2", L"InnerNamespace2"),
        };

        for (auto pair : cases) {
            shared_ptr<NamespaceDeclaration> declaration(dynamic_pointer_cast<NamespaceDeclaration>(metadataReader.findByName(pair.first)));
            Assert::IsTrue(declaration->name() == pair.second);
        }
    }

    TEST_METHOD(WindowsNamespaces) {
        MetadataReader metadataReader;

        auto cases = {
            pair<const wchar_t*, const wchar_t*>(L"Windows", L"Windows"),
            pair<const wchar_t*, const wchar_t*>(L"Windows.Foundation", L"Foundation"),
            pair<const wchar_t*, const wchar_t*>(L"Windows.Foundation.Collections", L"Collections"),
        };

        for (auto pair : cases) {
            shared_ptr<NamespaceDeclaration> declaration(dynamic_pointer_cast<NamespaceDeclaration>(metadataReader.findByName(pair.first)));
            Assert::IsTrue(declaration->name() == pair.second);
        }
    }

    TEST_METHOD(InvalidNamespaces) {
        MetadataReader metadataReader;

        Assert::IsTrue(metadataReader.findByName(L"Windows.") == nullptr);
        Assert::IsTrue(metadataReader.findByName(L".Windows") == nullptr);
    }
};

}
}