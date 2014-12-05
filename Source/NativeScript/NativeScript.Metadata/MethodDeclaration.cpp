#include "pch.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

// TODO
namespace {

vector<ParameterDeclaration> makeParameterDeclarations(ComPtr<IMetaDataImport2> metadata, mdMethodDef token) {
    HCORENUM enumerator{nullptr};
    ULONG count{0};
    array<mdParamDef, 1024> tokens;

    ASSERT_SUCCESS(metadata->EnumParams(&enumerator, token, tokens.data(), tokens.size(), &count));
    ASSERT(count < tokens.size() - 1);
    metadata->CloseEnum(enumerator);

    vector<ParameterDeclaration> result;
    for (size_t i = 0; i < count; ++i) {
        result.emplace_back(metadata, tokens[i]);
    }

    return result;
}

}

MethodDeclaration::MethodDeclaration(ComPtr<IMetaDataImport2> metadata, mdMethodDef token)
    : Base()
      , _metadata{metadata}
      , _token{token}
      , _parameters(makeParameterDeclarations(metadata, token)) {

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

IteratorRange<MethodDeclaration::ParameterIterator> MethodDeclaration::parameters() const {
    return IteratorRange<ParameterIterator>(_parameters.begin(), _parameters.end());
}

}
}
