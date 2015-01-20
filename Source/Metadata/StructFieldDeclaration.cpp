#include "pch.h"
#include "StructFieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    StructFieldDeclaration::StructFieldDeclaration(IMetaDataImport2* metadata, mdFieldDef token)
        : Base(DeclarationKind::StructField, metadata, token) {
    }

    PCCOR_SIGNATURE StructFieldDeclaration::signature() const {
        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };

        ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr, nullptr));

        ULONG header{ CorSigUncompressData(signature) };
        ASSERT(header == IMAGE_CEE_CS_CALLCONV_FIELD);

        return signature;
    }
}
}
