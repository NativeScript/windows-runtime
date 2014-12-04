#include "pch.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

MethodDeclaration::MethodDeclaration(ComPtr<IMetaDataImport2> metadata, mdMethodDef token)
    : Base()
      , _metadata{metadata}
      , _token{token} {

    ASSERT(token != mdMethodDefNil);
}

bool MethodDeclaration::isExported() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    if (!(IsMdPublic(methodFlags) || IsMdFamily(methodFlags))) {
        return false;
    }

    if (IsMdSpecialName(methodFlags) || IsMdRTSpecialName(methodFlags)) {
        return false;
    }

    return true;
}

bool MethodDeclaration::isStatic() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    return IsMdStatic(methodFlags) != 0;
}

wstring MethodDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring MethodDeclaration::fullName() const {
    mdTypeDef parentToken{mdTypeDefNil};

    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, &parentToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(name());
    return fullName;
}

}
}
