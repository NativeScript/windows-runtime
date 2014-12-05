#include "pch.h"
#include "ParameterDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

ParameterDeclaration::ParameterDeclaration(IMetaDataImport2* metadata, mdParamDef token)
    : Base()
    , _metadata{metadata}
    , _token{token} {

    ASSERT(token != mdParamDefNil);
}

wstring ParameterDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetParamProps(_token, nullptr, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring ParameterDeclaration::fullName() const {
    mdMethodDef methodToken{mdMethodDefNil};
    ASSERT_SUCCESS(_metadata->GetParamProps(_token, &methodToken, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr));

    mdTypeDef parentToken{mdTypeDefNil};

    identifier methodNameData;
    ULONG methodNameDataLength{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(methodToken, &parentToken, methodNameData.data(), methodNameData.size(), &methodNameDataLength, nullptr, nullptr, nullptr, nullptr, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};
    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(methodNameData.data(), methodNameDataLength - 1).append(L".").append(name());
    return fullName;
}

bool ParameterDeclaration::isIn() const {
    DWORD flags{0};
    ASSERT_SUCCESS(_metadata->GetParamProps(_token, nullptr, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr));

    return IsPdIn(flags) != 0;
}

bool ParameterDeclaration::isOut() const {
    DWORD flags{0};
    ASSERT_SUCCESS(_metadata->GetParamProps(_token, nullptr, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr));

    return IsPdOut(flags) != 0;
}

}
}
