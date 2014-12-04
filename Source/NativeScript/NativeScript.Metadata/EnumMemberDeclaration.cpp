#include "pch.h"
#include "EnumMemberDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

EnumMemberDeclaration::EnumMemberDeclaration(ComPtr<IMetaDataImport2> metadata, mdFieldDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {

    ASSERT(token != mdFieldDefNil);
}

wstring EnumMemberDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring EnumMemberDeclaration::fullName() const {
    mdTypeDef parentToken{mdTypeDefNil};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, &parentToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(name());
    return fullName;
}

int64_t EnumMemberDeclaration::value() const {
    UVCP_CONSTANT value{nullptr};
    DWORD valueType{0};

    ASSERT_SUCCESS(_metadata->GetFieldProps(_token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, &valueType, &value, nullptr));

    int64_t result{0};

    // TODO: Do this in the EnumDeclaration ctor.
    switch (valueType) {
        case ELEMENT_TYPE_I4:
            result = *static_cast<const int32_t*>(value);
            break;

        case ELEMENT_TYPE_U4:
            result = *static_cast<const uint32_t*>(value);
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return result;
}

}
}
