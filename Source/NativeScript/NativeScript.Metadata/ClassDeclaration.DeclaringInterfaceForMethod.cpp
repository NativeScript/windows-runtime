#include "pch.h"

#include "ClassDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

namespace {

mdTypeDef getContainingClassToken(IMetaDataImport2* metadata, const mdMethodDef methodToken) {
    mdTypeDef parent{mdTokenNil};
    ASSERT_SUCCESS(metadata->GetMethodProps(methodToken, &parent, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));
    return parent;
}

}

// Given a constructor searches for all methods of the activatable factories specified with the `ActivatableAttribute` with the same number of parameters.
// In WinRT there can't be an overload on constructors by argument type, so the factory method (if any) should be unique.
//
// [Activatable(456)]
// [Activatable(typeof(IActivatableClassFactory), 123)]
// [Activatable(typeof(IActivatableClassFactory2), 456)]
// public sealed class ActivatableClass : IActivatableClassClass
// {
//     public extern ActivatableClass();
//     public extern ActivatableClass([In] object arg1);
//     public extern ActivatableClass([In] object arg1, [In] object arg2);
// }
unique_ptr<InterfaceDeclaration> ClassDeclaration::declaringInterfaceForInstanceInitializer(const MethodDeclaration& instanceInitializer, size_t* outIndex) {
    size_t initializerArgumentsCount{instanceInitializer.numberOfParameters()};

    if (initializerArgumentsCount == 0) {
        *outIndex = SIZE_T_MAX;
        return nullptr;
    }

    IMetaDataImport2* metadata{instanceInitializer._metadata.Get()};
    mdMethodDef initializerToken{instanceInitializer._token};

    mdTypeDef classToken{getContainingClassToken(metadata, initializerToken)};

    array<mdCustomAttribute, 512> classAttributes;
    ULONG classAttributesCount{0};
    HCORENUM customAttributesEnumerator{nullptr};
    ASSERT_SUCCESS(metadata->EnumCustomAttributes(&customAttributesEnumerator, classToken, NULL, classAttributes.data(), classAttributes.size(), &classAttributesCount));
    ASSERT(classAttributesCount < classAttributes.size() - 1);
    metadata->CloseEnum(customAttributesEnumerator);

    for (size_t i = 0; i < classAttributesCount; ++i) {
        mdToken attributeConstructorToken{mdTokenNil};
        ASSERT_SUCCESS(metadata->GetCustomAttributeProps(classAttributes[i], nullptr, &attributeConstructorToken, nullptr, nullptr));

        mdTypeDef attributeClassToken{mdTokenNil};
        PCCOR_SIGNATURE attributeConstructorSignature{nullptr};
        switch (TypeFromToken(attributeConstructorToken)) {
            case mdtMethodDef:
                ASSERT_SUCCESS(metadata->GetMethodProps(attributeConstructorToken, &attributeClassToken, nullptr, 0, nullptr, nullptr, &attributeConstructorSignature, nullptr, nullptr, nullptr));
                break;

            case mdtMemberRef:
                ASSERT_SUCCESS(metadata->GetMemberRefProps(attributeConstructorToken, &attributeClassToken, nullptr, 0, nullptr, &attributeConstructorSignature, nullptr));
                break;

            default:
                ASSERT_NOT_REACHED();
        }

        identifier attributeClassName;
        switch (TypeFromToken(attributeClassToken)) {
            case mdtTypeDef:
                ASSERT_SUCCESS(metadata->GetTypeDefProps(attributeClassToken, attributeClassName.data(), attributeClassName.size(), nullptr, nullptr, nullptr));
                break;

            case mdtTypeRef:
                ASSERT_SUCCESS(metadata->GetTypeRefProps(attributeClassToken, nullptr, attributeClassName.data(), attributeClassName.size(), nullptr));
                break;

            default:
                ASSERT_NOT_REACHED();
        }

        if (wcscmp(attributeClassName.data(), ACTIVATABLE_ATTRIBUTE_W) != 0) {
            continue;
        }

        CorSigUncompressData(attributeConstructorSignature); // Skip Flags
        ULONG attributeConstructorArgumentCount{CorSigUncompressData(attributeConstructorSignature)};
        CorSigUncompressElementType(attributeConstructorSignature); // Skip Return type

        if (attributeConstructorArgumentCount == 0) {
            continue;
        }

        CorElementType attributeConstructorFirstParameterType{CorSigUncompressElementType(attributeConstructorSignature)};
        if (!(attributeConstructorFirstParameterType == ELEMENT_TYPE_CLASS && (attributeConstructorArgumentCount == 2 || attributeConstructorArgumentCount == 3))) {
            continue;
        }

        const uint8_t* attributeValue{nullptr};
        ULONG attributeValueSize{0};
        ASSERT_SUCCESS(metadata->GetCustomAttributeProps(classAttributes[i], nullptr, nullptr, reinterpret_cast<const void**>(&attributeValue), &attributeValueSize));

        wstring factoryName{getStringValueFromBlob(metadata, attributeValue + 2)};
        mdTypeDef factoryToken{mdTokenNil};
        ASSERT_SUCCESS(metadata->FindTypeDefByName(factoryName.data(), NULL, &factoryToken));

        array<mdMethodDef, 512> factoryMethods;
        ULONG factoryMethodsCount{0};
        HCORENUM factoryMethodsEnumerator{nullptr};
        ASSERT_SUCCESS(metadata->EnumMethods(&factoryMethodsEnumerator, factoryToken, factoryMethods.data(), factoryMethods.size(), &factoryMethodsCount));
        ASSERT(factoryMethodsCount < factoryMethods.size() - 1);
        metadata->CloseEnum(factoryMethodsEnumerator);

        for (size_t j = 0; j < factoryMethodsCount; ++j) {
            PCCOR_SIGNATURE factoryMethodSignature{nullptr};
            ASSERT_SUCCESS(metadata->GetMethodProps(factoryMethods[j], nullptr, nullptr, 0, nullptr, nullptr, &factoryMethodSignature, nullptr, nullptr, nullptr));

            CorSigUncompressData(factoryMethodSignature); // Skip flags
            ULONG factoryMethodParametersCount{CorSigUncompressData(factoryMethodSignature)};

            if (factoryMethodParametersCount != initializerArgumentsCount) {
                continue;
            }

            *outIndex = j;
            return make_unique<InterfaceDeclaration>(metadata, factoryToken);
        }
    }

    ASSERT_NOT_REACHED();
}

unique_ptr<InterfaceDeclaration> ClassDeclaration::declaringInterfaceForMethod(const MethodDeclaration& method, size_t* outIndex) {
    ASSERT(outIndex);

    if (method.isInitializer()) {
        return declaringInterfaceForInstanceInitializer(method, outIndex);
    }

    if (method.isStatic()) {
        NOT_IMPLEMENTED();
    } else {
        NOT_IMPLEMENTED();
    }
}

}
}
