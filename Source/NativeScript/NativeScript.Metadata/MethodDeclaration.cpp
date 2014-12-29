#include "pch.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

const wchar_t* const OVERLOAD_ATTRIBUTE_W{L"Windows.Foundation.Metadata.OverloadAttribute"};
const wchar_t* const DEFAULT_OVERLOAD_ATTRIBUTE_W{L"Windows.Foundation.Metadata.DefaultOverloadAttribute"};

// TODO
namespace {

vector<ParameterDeclaration> makeParameterDeclarations(IMetaDataImport2* metadata, mdMethodDef token) {
    HCORENUM enumerator{nullptr};
    ULONG count{0};
    array<mdParamDef, 1024> tokens;

    ASSERT_SUCCESS(metadata->EnumParams(&enumerator, token, tokens.data(), tokens.size(), &count));
    ASSERT(count < tokens.size() - 1);
    metadata->CloseEnum(enumerator);

    size_t parameterStartIndex{0};

    // Skip return parameter
    if (count > 0) {
        ULONG firstParameterIndex{0};
        ASSERT_SUCCESS(metadata->GetParamProps(tokens[0], nullptr, &firstParameterIndex, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr));

        if (firstParameterIndex == 0) {
            parameterStartIndex = 1;
        }
    }

    vector<ParameterDeclaration> result;
    for (size_t i = parameterStartIndex; i < count; ++i) {
        result.emplace_back(metadata, tokens[i]);
    }

    return result;
}

}

MethodDeclaration::MethodDeclaration(IMetaDataImport2* metadata, mdMethodDef token)
    : Base()
    , _metadata{metadata}
    , _token{token}
    , _parameters(makeParameterDeclarations(metadata, token)) {

    ASSERT(metadata);
    ASSERT(TypeFromToken(token) == mdtMethodDef);
    ASSERT(token != mdMethodDefNil);
}

bool MethodDeclaration::isExported() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    if (!(IsMdPublic(methodFlags) || IsMdFamily(methodFlags) || IsMdFamORAssem(methodFlags))) {
        return false;
    }

    if (IsMdSpecialName(methodFlags)) {
        return false;
    }

    return true;
}

bool MethodDeclaration::isStatic() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    return IsMdStatic(methodFlags) != 0;
}

bool MethodDeclaration::isOverridable() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    return !IsMdStatic(methodFlags) && !IsMdFinal(methodFlags);
}

wstring MethodDeclaration::name() const {
    return fullName();
}

wstring MethodDeclaration::fullName() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

bool MethodDeclaration::isInitializer() const {
    identifier name;
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, name.data(), name.size(), nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    return IsMdInstanceInitializerW(methodFlags, name.data());
}

IteratorRange<MethodDeclaration::ParameterIterator> MethodDeclaration::parameters() const {
    return IteratorRange<ParameterIterator>(_parameters.begin(), _parameters.end());
}

size_t MethodDeclaration::numberOfParameters() const {
    PCCOR_SIGNATURE signature{nullptr};
    ULONG signatureSize{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr));

    CorSigUncompressData(signature);
    ULONG numberOfArguments{CorSigUncompressData(signature)};

#if _DEBUG
    ptrdiff_t numberOfParameters{distance(parameters().begin(), parameters().end())};
    ASSERT(numberOfArguments == numberOfParameters);
#endif

    return numberOfArguments;
}

wstring MethodDeclaration::overloadName() const {
    return getUnaryCustomAttributeStringValue(_metadata.Get(), _token, OVERLOAD_ATTRIBUTE_W);
}

bool MethodDeclaration::isDefaultOverload() const {
    HRESULT getAttributeResult{_metadata->GetCustomAttributeByName(_token, DEFAULT_OVERLOAD_ATTRIBUTE_W, nullptr, nullptr)};
    ASSERT_SUCCESS(getAttributeResult);
    return getAttributeResult == S_OK;
}

}
}
