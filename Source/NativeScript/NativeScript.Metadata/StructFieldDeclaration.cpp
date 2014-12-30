#include "pch.h"
#include "StructFieldDeclaration.h"
#include "EnumDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

StructFieldDeclaration::StructFieldDeclaration(IMetaDataImport2* metadata, mdFieldDef token)
    : Base(metadata, token) {

}

CorElementType StructFieldDeclaration::type() const {
    PCCOR_SIGNATURE signature{nullptr};
    ULONG signatureSize{0};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr, nullptr));

    ULONG header{CorSigUncompressData(signature)};
    ASSERT(header == IMAGE_CEE_CS_CALLCONV_FIELD);

    ULONG firstElement{0};
    CorSigUncompressData(signature, &firstElement);

    if (firstElement < ELEMENT_TYPE_PTR) {
        return static_cast<CorElementType>(firstElement);
    }

    // Enum
    if (firstElement == ELEMENT_TYPE_VALUETYPE) {
        CorSigUncompressData(signature);
        mdToken token{CorSigUncompressToken(signature)};

        identifier enumName;
        switch (TypeFromToken(token)) {
            case mdtTypeRef: {
                ASSERT_SUCCESS(_metadata->GetTypeRefProps(token, nullptr, enumName.data(), enumName.size(), nullptr));
                break;
            }

            case mdtTypeDef: {
                ASSERT_SUCCESS(_metadata->GetTypeDefProps(token, enumName.data(), enumName.size(), nullptr, nullptr, nullptr));
                break;
            }

            default:
                ASSERT_NOT_REACHED();
        }

        ComPtr<IMetaDataImport2> enumMetadata;
        mdTypeDef enumToken{mdTokenNil};
        ASSERT_SUCCESS(RoGetMetaDataFile(HStringReference(enumName.data()).Get(), nullptr, nullptr, enumMetadata.GetAddressOf(), &enumToken));

        EnumDeclaration enumeration{enumMetadata.Get(), enumToken};
        return enumeration.type();
    }

    ASSERT_NOT_REACHED();
}

}
}
