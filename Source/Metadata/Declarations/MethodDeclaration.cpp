#include "Metadata-Prefix.h"
#include "MethodDeclaration.h"
#include "Signature.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    const wchar_t* const OVERLOAD_ATTRIBUTE_W{ L"Windows.Foundation.Metadata.OverloadAttribute" };
    const wchar_t* const DEFAULT_OVERLOAD_ATTRIBUTE_W{ L"Windows.Foundation.Metadata.DefaultOverloadAttribute" };

    MethodDeclaration::MethodDeclaration(IMetaDataImport2* metadata, mdMethodDef token)
        : Base(DeclarationKind::Method)
        , _metadata{ metadata }
        , _token{ token }
        , _parameters() {

        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtMethodDef);
        ASSERT(token != mdMethodDefNil);

        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };

        ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr));

#if _DEBUG
        PCCOR_SIGNATURE startSignature{ signature };
#endif

        if (CorSigUncompressCallingConv(signature) == IMAGE_CEE_CS_CALLCONV_GENERIC) {
            NOT_IMPLEMENTED();
        }

        ULONG argumentsCount{ CorSigUncompressData(signature) };

        _returnType = Signature::consumeType(signature);

        HCORENUM parameterEnumerator{ nullptr };
        ULONG parametersCount{ 0 };
        array<mdParamDef, 1024> parameterTokens;

        ASSERT_SUCCESS(_metadata->EnumParams(&parameterEnumerator, _token, parameterTokens.data(), parameterTokens.size(), &parametersCount));
        ASSERT(parametersCount < parameterTokens.size() - 1);
        _metadata->CloseEnum(parameterEnumerator);

        size_t startIndex{ 0 };
        if (argumentsCount + 1 == parametersCount) {
            ++startIndex;
        }

        for (size_t i = startIndex; i < parametersCount; ++i) {
            PCCOR_SIGNATURE type{ Signature::consumeType(signature) };
            _parameters.emplace_back(_metadata.Get(), parameterTokens[i], type);
        }

        ASSERT(startSignature + signatureSize == signature);
    }

    bool MethodDeclaration::isExported() const {
        DWORD methodFlags{ 0 };
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
        DWORD methodFlags{ 0 };
        ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

        return IsMdStatic(methodFlags) != 0;
    }

    bool MethodDeclaration::isSealed() const {
        DWORD methodFlags{ 0 };
        ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

        return IsMdStatic(methodFlags) || IsMdFinal(methodFlags);
    }

    PCCOR_SIGNATURE MethodDeclaration::returnType() const {
        return _returnType;
    }

    wstring MethodDeclaration::name() const {
        return fullName();
    }

    wstring MethodDeclaration::fullName() const {
        identifier nameData;
        ULONG nameLength{ 0 };

        ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr));

        wstring result{ nameData.data(), nameLength - 1 };
        return result;
    }

    bool MethodDeclaration::isInitializer() const {
        identifier name;
        DWORD methodFlags{ 0 };
        ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, name.data(), name.size(), nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

        return IsMdInstanceInitializerW(methodFlags, name.data());
    }

    IteratorRange<MethodDeclaration::ParameterIterator> MethodDeclaration::parameters() const {
        return IteratorRange<ParameterIterator>(_parameters.begin(), _parameters.end());
    }

    size_t MethodDeclaration::numberOfParameters() const {
        return distance(parameters().begin(), parameters().end());
    }

    wstring MethodDeclaration::overloadName() const {
        return getUnaryCustomAttributeStringValue(_metadata.Get(), _token, OVERLOAD_ATTRIBUTE_W);
    }

    bool MethodDeclaration::isDefaultOverload() const {
        HRESULT getAttributeResult{ _metadata->GetCustomAttributeByName(_token, DEFAULT_OVERLOAD_ATTRIBUTE_W, nullptr, nullptr) };
        ASSERT_SUCCESS(getAttributeResult);
        return getAttributeResult == S_OK;
    }
}
}
