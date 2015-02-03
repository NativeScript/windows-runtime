#pragma once

#include <memory>
#include "Declarations/Declarations.h"

namespace NativeScript {
namespace Metadata {
    class DeclarationFactory final {
    public:
        static std::unique_ptr<TypeDeclaration> makeTypeDefDeclaration(IMetaDataImport2*, mdTypeDef, const wchar_t* fullName);

        static std::unique_ptr<NamespaceDeclaration> makeNamespaceDeclaration(const wchar_t* fullName);

        static std::unique_ptr<DelegateDeclaration> makeDelegateDeclaration(IMetaDataImport2*, mdToken);

        static std::unique_ptr<InterfaceDeclaration> makeInterfaceDeclaration(IMetaDataImport2*, mdToken);
    };
}
}
