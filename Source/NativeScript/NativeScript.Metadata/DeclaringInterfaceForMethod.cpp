#include "pch.h"

#include "ClassDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

namespace {

mdToken getMethodContainingClassToken(IMetaDataImport2* metadata, mdMethodDef methodToken) {
    mdToken classToken{mdTokenNil};

    switch (TypeFromToken(methodToken)) {
        case mdtMethodDef:
            ASSERT_SUCCESS(metadata->GetMethodProps(methodToken, &classToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));
            break;

        case mdtMemberRef:
            ASSERT_SUCCESS(metadata->GetMemberRefProps(methodToken, &classToken, nullptr, 0, nullptr, nullptr, nullptr));
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return classToken;
}

mdToken getCustomAttributeConstructorToken(IMetaDataImport2* metadata, mdCustomAttribute customAttribute) {
    mdToken constructorToken{mdTokenNil};
    ASSERT_SUCCESS(metadata->GetCustomAttributeProps(customAttribute, nullptr, &constructorToken, nullptr, nullptr));
    return constructorToken;
}

mdToken getCustomAttributeClassToken(IMetaDataImport2* metadata, mdCustomAttribute customAttribute) {
    return getMethodContainingClassToken(metadata, getCustomAttributeConstructorToken(metadata, customAttribute));
}

identifier getClassName(IMetaDataImport2* metadata, const mdToken token) {
    identifier attributeClassName;

    switch (TypeFromToken(token)) {
        case mdtTypeDef:
            ASSERT_SUCCESS(metadata->GetTypeDefProps(token, attributeClassName.data(), attributeClassName.size(), nullptr, nullptr, nullptr));
            break;

        case mdtTypeRef:
            ASSERT_SUCCESS(metadata->GetTypeRefProps(token, nullptr, attributeClassName.data(), attributeClassName.size(), nullptr));
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return attributeClassName;
}

PCCOR_SIGNATURE getMethodSignature(IMetaDataImport2* metadata, mdMethodDef token) {
    PCCOR_SIGNATURE signature{nullptr};

    switch (TypeFromToken(token)) {
        case mdtMethodDef:
            ASSERT_SUCCESS(metadata->GetMethodProps(token, nullptr, nullptr, 0, nullptr, nullptr, &signature, nullptr, nullptr, nullptr));
            break;

        case mdtMemberRef:
            ASSERT_SUCCESS(metadata->GetMemberRefProps(token, nullptr, nullptr, 0, nullptr, &signature, nullptr));
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return signature + 1;
}

ULONG getSignatureArgumentCount(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
    ULONG argumentCount{CorSigUncompressData(signature)};
    return argumentCount;
}

size_t getMethodArgumentCount(IMetaDataImport2* metadata, mdToken token) {
    return getSignatureArgumentCount(metadata, getMethodSignature(metadata, token));
}

vector<mdCustomAttribute> getCustomAttributesWithName(IMetaDataImport2* metadata, mdTypeDef token, const wchar_t* attributeName) {
    array<mdCustomAttribute, 512> attributes;
    ULONG attributesCount{0};
    HCORENUM attributesEnumerator{nullptr};

    ASSERT_SUCCESS(metadata->EnumCustomAttributes(&attributesEnumerator, token, NULL, attributes.data(), attributes.size(), &attributesCount));
    ASSERT(attributesCount < attributes.size() - 1);
    metadata->CloseEnum(attributesEnumerator);

    vector<mdCustomAttribute> filteredAttributes;

    for (size_t i = 0; i < attributesCount; ++i) {
        mdCustomAttribute attribute{attributes[i]};
        mdToken classAttributeClassToken{getCustomAttributeClassToken(metadata, attribute)};
        identifier classAttributeClassName = getClassName(metadata, classAttributeClassToken);

        if (wcscmp(classAttributeClassName.data(), attributeName) != 0) {
            continue;
        }

        filteredAttributes.push_back(attribute);
    }

    return filteredAttributes;
}

mdTypeDef getCustomAttributeTypeArgument(IMetaDataImport2* metadata, mdCustomAttribute token) {
    const void* attributeValue{nullptr};
    ULONG attributeValueSize{0};
    ASSERT_SUCCESS(metadata->GetCustomAttributeProps(token, nullptr, nullptr, &attributeValue, &attributeValueSize));

    wstring typeName{getStringValueFromBlob(metadata, static_cast<PCCOR_SIGNATURE>(attributeValue) + 2)};

    mdTypeDef typeToken{mdTokenNil};
    ASSERT_SUCCESS(metadata->FindTypeDefByName(typeName.data(), NULL, &typeToken));
    return typeToken;
}

vector<mdMethodDef> getClassMethods(IMetaDataImport2* metadata, mdTypeDef token) {
    vector<mdMethodDef> methods(1024);

    ULONG methodsCount{0};
    HCORENUM factoryMethodsEnumerator{nullptr};
    ASSERT_SUCCESS(metadata->EnumMethods(&factoryMethodsEnumerator, token, methods.data(), methods.size(), &methodsCount));
    ASSERT(methodsCount < methods.size() - 1);
    metadata->CloseEnum(factoryMethodsEnumerator);

    methods.resize(methodsCount);
    return methods;
}

bool hasMethodFirstTypeArgument(IMetaDataImport2* metadata, mdToken token) {
    PCCOR_SIGNATURE signature{getMethodSignature(metadata, token)};

    ULONG argumentCount{CorSigUncompressData(signature)};
    if (argumentCount == 0) {
        return false;
    }

    CorElementType returnType{CorSigUncompressElementType(signature)};
    ASSERT(returnType == ELEMENT_TYPE_VOID);

    CorElementType firstArgument{CorSigUncompressElementType(signature)};
    if (firstArgument != ELEMENT_TYPE_CLASS) {
        return false;
    }

    mdToken firstArgumentToken{CorSigUncompressToken(signature)};
    identifier firstArgumentTypeName = getClassName(metadata, firstArgumentToken);

    if (wcscmp(firstArgumentTypeName.data(), SYSTEM_TYPE_W) != 0) {
        return false;
    }

    return true;
}

// TODO: Search Activatable
unique_ptr<const InterfaceDeclaration> declaringInterfaceForInitializer(IMetaDataImport2* metadata, mdMethodDef methodToken, size_t* outIndex) {
    size_t methodArgumentCount{getMethodArgumentCount(metadata, methodToken)};
    if (methodArgumentCount == 0) {
        *outIndex = SIZE_T_MAX;
        return nullptr;
    }

    mdTypeDef classToken{getMethodContainingClassToken(metadata, methodToken)};
    ASSERT(TypeFromToken(classToken) == mdtTypeDef);

    vector<mdCustomAttribute> activatableAttributes{getCustomAttributesWithName(metadata, classToken, ACTIVATABLE_ATTRIBUTE_W)};
    for (const mdCustomAttribute& attributeToken : activatableAttributes) {
        mdToken attributeConstructorToken{getCustomAttributeConstructorToken(metadata, attributeToken)};

        if (!hasMethodFirstTypeArgument(metadata, attributeConstructorToken)) {
            continue;
        }

        mdTypeDef factoryToken{getCustomAttributeTypeArgument(metadata, attributeToken)};

        vector<mdMethodDef> factoryMethods{getClassMethods(metadata, factoryToken)};
        for (size_t i = 0; i < factoryMethods.size(); ++i) {
            mdMethodDef factoryMethod{factoryMethods[i]};

            ULONG factoryMethodArgumentsCount{getMethodArgumentCount(metadata, factoryMethod)};
            if (factoryMethodArgumentsCount != methodArgumentCount) {
                continue;
            }

            *outIndex = i;
            return make_unique<InterfaceDeclaration>(metadata, factoryToken);
        }
    }

    ASSERT_NOT_REACHED();
}

unique_ptr<InterfaceDeclaration> declaringInterfaceForStaticMethod(IMetaDataImport2* metadata, mdMethodDef methodToken, size_t* outIndex) {
    PCCOR_SIGNATURE methodSignature{getMethodSignature(metadata, methodToken)};

    mdTypeDef classToken{getMethodContainingClassToken(metadata, methodToken)};
    ASSERT(TypeFromToken(classToken) == mdtTypeDef);

    vector<mdCustomAttribute> staticAttributes{getCustomAttributesWithName(metadata, classToken, STATIC_ATTRIBUTE_W)};
    for (const mdCustomAttribute& attributeToken : staticAttributes) {
        mdTypeDef staticsToken{getCustomAttributeTypeArgument(metadata, attributeToken)};

        vector<mdMethodDef> staticMethods{getClassMethods(metadata, staticsToken)};
        for (size_t i = 0; i < staticMethods.size(); ++i) {
            mdMethodDef staticMethod{staticMethods[i]};
            PCCOR_SIGNATURE staticSignature{nullptr};
            ULONG staticSignatureSize{0};

            ASSERT_SUCCESS(metadata->GetMethodProps(staticMethod, nullptr, nullptr, 0, nullptr, nullptr, &staticSignature, &staticSignatureSize, nullptr, nullptr));

            if (memcmp(staticSignature + 1, methodSignature, staticSignatureSize - 1) != 0) {
                continue;
            }

            *outIndex = i;
            return make_unique<InterfaceDeclaration>(metadata, staticsToken);
        }
    }

    ASSERT_NOT_REACHED();
}

}

unique_ptr<const InterfaceDeclaration> findDeclaringInterfaceForMethod(const MethodDeclaration& method, size_t* outIndex) {
    ASSERT(outIndex);

    IMetaDataImport2* metadata{method._metadata.Get()};
    mdMethodDef methodToken{method._token};

    if (method.isStatic()) {
        return declaringInterfaceForStaticMethod(metadata, methodToken, outIndex);
    } else {
        if (method.isInitializer()) {
            return declaringInterfaceForInitializer(metadata, methodToken, outIndex);
        } else {
            NOT_IMPLEMENTED();
        }
    }
}

}
}
