#include "Metadata-Prefix.h"
#include "StructFieldDeclaration.h"
#include "TypeCache.h"
#include "Signature.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    StructFieldDeclaration::StructFieldDeclaration(IMetaDataImport2* metadata, mdFieldDef token)
        : Base(metadata, token) {
    }

    const Type& StructFieldDeclaration::type() const {
        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };

        ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr, nullptr));

#if _DEBUG
        PCCOR_SIGNATURE startSignature{ signature };
#endif

        ULONG header{ CorSigUncompressData(signature) };
        ASSERT(header == IMAGE_CEE_CS_CALLCONV_FIELD);

        const Type& fieldType{ MetadataReader::parseType(_metadata.Get(), consumeType(signature)) };
        ASSERT(startSignature + signatureSize == signature);

        return fieldType;
    }
}
}
