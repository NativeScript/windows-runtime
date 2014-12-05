#include "pch.h"
#include "FieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

FieldDeclaration::FieldDeclaration(ComPtr<IMetaDataImport2> metadata, mdFieldDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {

    ASSERT(token != mdFieldDefNil);
}

wstring FieldDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring FieldDeclaration::fullName() const {
    mdTypeDef parentToken{mdTypeDefNil};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, &parentToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(name());
    return fullName;
}

}
}
