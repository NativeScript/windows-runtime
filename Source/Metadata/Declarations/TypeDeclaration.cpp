#include "Metadata-Prefix.h"
#include "TypeDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    TypeDeclaration::TypeDeclaration(DeclarationKind kind, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(kind)
        , _metadata{ metadata }
        , _token{ token } {

        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtTypeDef);
        ASSERT(token != mdTypeDefNil);
    }

    bool TypeDeclaration::isExported() const {
        DWORD flags{ 0 };
        ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, nullptr, 0, nullptr, &flags, nullptr));

        if (!IsTdPublic(flags) || IsTdSpecialName(flags)) {
            return false;
        }

        return true;
    }

    wstring TypeDeclaration::name() const {
        wstring fullyQualifiedName(fullName());

        size_t backtickIndex{ fullyQualifiedName.rfind(L"`") };
        if (backtickIndex != wstring::npos) {
            fullyQualifiedName.erase(backtickIndex);
        }

        size_t dotIndex{ fullyQualifiedName.rfind(L".") };
        if (dotIndex != wstring::npos) {
            fullyQualifiedName.erase(0, dotIndex + 1);
        }

        return fullyQualifiedName;
    }

    wstring TypeDeclaration::fullName() const {
        identifier fullNameData;
        ULONG fullNameDataLength{ 0 };

        ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, fullNameData.data(), fullNameData.size(), &fullNameDataLength, nullptr, nullptr));

        wstring fullName{ fullNameData.data(), fullNameDataLength - 1 };
        return fullName;
    }
}
}
