#include "Metadata-Prefix.h"
#include "NamespaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;

    NamespaceDeclaration::NamespaceDeclaration(const wchar_t* fullName)
        : Base(DeclarationKind::Namespace)
        , _fullName(fullName) {

        ASSERT(fullName);

        HSTRING* namespaces{ nullptr };
        DWORD namespacesCount{ 0 };

        // RoResolveNamespace gives incomplete results, find a better way.
        HRESULT hr{ RoResolveNamespace(HStringReference(_fullName.data()).Get(), nullptr, 0, nullptr, nullptr, nullptr, &namespacesCount, &namespaces) };

        // The search for the "Windows" namespace on Windows Phone 8.1 fails both on a device and on an emulator with corrupted metadata error.
        if (FAILED(hr)) {
            return;
        }

        for (size_t i = 0; i < namespacesCount; ++i) {
            _children.emplace_back(WindowsGetStringRawBuffer(namespaces[i], nullptr));
            WindowsDeleteString(namespaces[i]);
        }

        CoTaskMemFree(namespaces);
    }

    wstring NamespaceDeclaration::name() const {
        wstring fullyQualifiedName(fullName());
        size_t dotIndex{ fullyQualifiedName.rfind(L".") };
        if (dotIndex != wstring::npos) {
            fullyQualifiedName = fullyQualifiedName.substr(dotIndex + 1);
        }
        return fullyQualifiedName;
    }

    wstring NamespaceDeclaration::fullName() const {
        return _fullName;
    }

    IteratorRange<NamespaceDeclaration::ChildrenIterator> NamespaceDeclaration::children() {
        return IteratorRange<ChildrenIterator>(_children.begin(), _children.end());
    }
}
}
