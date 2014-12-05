#include "pch.h"
#include "PropertyDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

PropertyDeclaration::PropertyDeclaration(IMetaDataImport2* metadata, mdFieldDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {

    ASSERT(token != mdPropertyNil);
}

bool PropertyDeclaration::isExported() const {
    DWORD propertyFlags{0};
    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, &propertyFlags, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    if (IsPrSpecialName(propertyFlags) || IsPrRTSpecialName(propertyFlags)) {
        return false;
    }

    return true;
}

wstring PropertyDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring PropertyDeclaration::fullName() const {
    mdTypeDef parentToken{mdTypeDefNil};

    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, &parentToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(name());
    return fullName;
}

bool PropertyDeclaration::isStatic() const {
    PCCOR_SIGNATURE signature{nullptr};
    ULONG signatureCount{0};

    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    ASSERT(signatureCount > 0);

    return (signature[0] & IMAGE_CEE_CS_CALLCONV_HASTHIS) == 0;
}

unique_ptr<MethodDeclaration> PropertyDeclaration::getter() const {
    mdMethodDef getterToken{mdMethodDefNil};

    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &getterToken, nullptr, 0, nullptr));

    if (getterToken == mdMethodDefNil) {
        return nullptr;
    }

    return make_unique<MethodDeclaration>(_metadata.Get(), getterToken);
}

unique_ptr<MethodDeclaration> PropertyDeclaration::setter() const {
    mdMethodDef setterToken{mdMethodDefNil};

    ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &setterToken, nullptr, nullptr, 0, nullptr));

    if (setterToken == mdMethodDefNil) {
        return nullptr;
    }

    return make_unique<MethodDeclaration>(_metadata.Get(), setterToken);
}

}
}
