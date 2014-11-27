#include "pch.h"
#include "MetadataDeclaration.h"

namespace NativeScript {
namespace Metadata {
namespace Ast {

using namespace std;
using namespace Microsoft::WRL;

MetadataDeclaration::MetadataDeclaration(const ComPtr<IMetaDataImport2>& metadata, const mdTypeDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {
}

wstring MetadataDeclaration::name() const {
    identifier fullyQualifiedNameData;
    ULONG fullyQualifiedNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, fullyQualifiedNameData.data(), fullyQualifiedNameData.size(), &fullyQualifiedNameDataLength, nullptr, nullptr));

    wstring fullyQualifiedName(fullyQualifiedNameData.data(), fullyQualifiedNameDataLength - 1);
    wstring name(fullyQualifiedName.substr(fullyQualifiedName.rfind(L".") + 1));
    return name;
}

}
}
}