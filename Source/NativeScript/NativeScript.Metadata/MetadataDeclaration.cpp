#include "pch.h"
#include "MetadataDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

MetadataDeclaration::MetadataDeclaration(ComPtr<IMetaDataImport2> metadata, mdTypeDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {

    ASSERT(token != mdTypeDefNil);
}

wstring MetadataDeclaration::name() const {
    wstring fullyQualifiedName(fullName());
    size_t dotIndex = fullyQualifiedName.rfind(L".");
    if (dotIndex != wstring::npos) {
        fullyQualifiedName = fullyQualifiedName.substr(dotIndex + 1);
    }
    return fullyQualifiedName;
}

wstring MetadataDeclaration::fullName() const {
    identifier fullNameData;
    ULONG fullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, fullNameData.data(), fullNameData.size(), &fullNameDataLength, nullptr, nullptr));

    wstring fullName(fullNameData.data(), fullNameDataLength - 1);
    return fullName;
}

}
}
