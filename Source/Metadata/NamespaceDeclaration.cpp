#include "Metadata-Prefix.h"
#include "NamespaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    NamespaceDeclaration::NamespaceDeclaration(const wchar_t* fullName)
        : Base(DeclarationKind::Namespace)
        , _fullName(fullName) {

        ASSERT(fullName);
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
}
}
