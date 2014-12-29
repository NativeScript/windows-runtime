#include "pch.h"
#include "EventDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

EventDeclaration::EventDeclaration(IMetaDataImport2* metadata, mdEvent token)
    : _metadata{metadata}
    , _token{token} {

    ASSERT(metadata);
    ASSERT(TypeFromToken(token) == mdtEvent);
    ASSERT(token != mdEventNil);
}

bool EventDeclaration::isExported() const {
    DWORD flags{0};
    ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    if (IsEvSpecialName(flags)) {
        return false;
    }

    return true;
}

wstring EventDeclaration::name() const {
    return fullName();
}

wstring EventDeclaration::fullName() const {
    identifier nameData;
    ULONG nameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nameData.data(), nameData.size(), &nameDataLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    wstring name{nameData.data(), nameDataLength - 1};
    return name;
}

bool EventDeclaration::isStatic() const {
    return addMethod().isStatic();
}

bool EventDeclaration::isOverridable() const {
    return addMethod().isOverridable();
}

DelegateDeclaration EventDeclaration::type() const {
    mdToken delegateToken{mdTokenNil};

    ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &delegateToken, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

    switch (TypeFromToken(delegateToken)) {
        case mdtTypeDef: {
            return DelegateDeclaration(_metadata.Get(), delegateToken);
        }

        case mdtTypeRef: {
            ComPtr<IMetaDataImport2> externalMetadata;
            mdTypeDef externalDelegateToken{mdTokenNil};

            bool isResolved{resolveTypeRef(_metadata.Get(), delegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken)};
            ASSERT(isResolved);

            return DelegateDeclaration(externalMetadata.Get(), externalDelegateToken);
        }

        case mdtTypeSpec: {
            PCCOR_SIGNATURE signature{nullptr};
            ULONG signatureSize{0};
            ASSERT_SUCCESS(_metadata->GetTypeSpecFromToken(delegateToken, &signature, &signatureSize));

            ULONG type1{CorSigUncompressData(signature)};
            ASSERT(type1 == ELEMENT_TYPE_GENERICINST);

            ULONG type2{CorSigUncompressData(signature)};
            ASSERT(type2 == ELEMENT_TYPE_CLASS);

            mdToken openGenericDelegateToken{CorSigUncompressToken(signature)};
            switch (TypeFromToken(openGenericDelegateToken)) {
                case mdtTypeDef: {
                    return DelegateDeclaration(_metadata.Get(), openGenericDelegateToken);
                }

                case mdtTypeRef: {
                    ComPtr<IMetaDataImport2> externalMetadata;
                    mdTypeDef externalDelegateToken{mdTokenNil};

                    bool isResolved{resolveTypeRef(_metadata.Get(), openGenericDelegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken)};
                    ASSERT(isResolved);

                    return DelegateDeclaration(externalMetadata.Get(), externalDelegateToken);
                }

                default:
                    ASSERT_NOT_REACHED();
            }
        }

        default:
            ASSERT_NOT_REACHED();
    }
}

MethodDeclaration EventDeclaration::addMethod() const {
    mdMethodDef addMethodToken{mdTokenNil};
    ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, &addMethodToken, nullptr, nullptr, nullptr, 0, nullptr));

    return MethodDeclaration{_metadata.Get(), addMethodToken};
}

MethodDeclaration EventDeclaration::removeMethod() const {
    mdMethodDef removeMethodToken{mdTokenNil};
    ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, &removeMethodToken, nullptr, nullptr, 0, nullptr));

    return MethodDeclaration{_metadata.Get(), removeMethodToken};
}

}
}
