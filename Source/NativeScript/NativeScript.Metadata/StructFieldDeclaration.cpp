#include "pch.h"
#include "StructFieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

StructFieldDeclaration::StructFieldDeclaration(IMetaDataImport2* metadata, mdFieldDef token)
    : Base(metadata, token) {

}

PCCOR_SIGNATURE StructFieldDeclaration::signature() const {
    PCCOR_SIGNATURE signature{nullptr};
    ULONG signatureSize{0};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr, nullptr));
    
    ASSERT(signature);
    ASSERT(signatureSize > 1);

    ASSERT(signature[0] == IMAGE_CEE_CS_CALLCONV_FIELD);
    return signature + 1;
}

}
}
