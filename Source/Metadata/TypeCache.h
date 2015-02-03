#pragma once

#include <memory>
#include <unordered_map>

#include <cor.h>
#include "Metadata-Prefix.h"

#include "Declarations/Declarations.h"
#include "Types/Types.h"
#include "DeclarationFactory.h"
#include "Signature.h"
#include "MetadataReader.h"

namespace NativeScript {
namespace Metadata {

    // TODO: Check double add !!!
    class TypeCache final {
    public:
        const Type& voidType() const { return _voidType; }
        const Type& booleanType() const { return _booleanType; }
        const Type& char16Type() const { return _char16Type; }
        const Type& int8Type() const { return _int8Type; }
        const Type& uInt8Type() const { return _uInt8Type; }
        const Type& int16Type() const { return _int16Type; }
        const Type& uInt16Type() const { return _uInt16Type; }
        const Type& int32Type() const { return _int32Type; }
        const Type& uInt32Type() const { return _uInt32Type; }
        const Type& int64Type() const { return _int64Type; }
        const Type& uInt64Type() const { return _uInt64Type; }
        const Type& singleType() const { return _singleType; }
        const Type& doubleType() const { return _doubleType; }
        const Type& stringType() const { return _stringType; }
        const Type& objectType() const { return _objectType; }
        const Type& guidType() const { return _guidType; }

        const Type& getOrCreateNamespace(const wchar_t* fullName) {
            auto existing = _types.find(fullName);
            if (existing != _types.end()) {
                return *existing->second.get();
            }

            const NamespaceDeclaration* namespaceDeclaration{ DeclarationFactory::makeNamespaceDeclaration(fullName).release() };

            _types.emplace(namespaceDeclaration->fullName(), std::unique_ptr<const NamespaceDeclaration>(namespaceDeclaration));
            return *namespaceDeclaration;
        }

        const Type& getOrCreateTypeDeclaration(IMetaDataImport2* metadata, mdToken token) {
            ASSERT(metadata);
            ASSERT(token != mdTokenNil);

            // TODO: Remove wstring
            std::wstring typeName{ getTypeName(metadata, token) };

            auto existing = _types.find(typeName);
            if (existing != _types.end()) {
                return *existing->second.get();
            }

            TypeDeclaration* typeDeclaration{ nullptr };

            switch (TypeFromToken(token)) {
            case mdtTypeDef: {
                typeDeclaration = DeclarationFactory::makeTypeDefDeclaration(metadata, token, typeName.data()).release();
                break;
            }

            case mdtTypeRef: {
                if (typeName == L"System.Object") {
                    return _objectType;
                }

                if (typeName == L"System.Guid") {
                    return _guidType;
                }

                Microsoft::WRL::ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, token, externalMetadata.GetAddressOf(), &externalToken) };
                ASSERT(isResolved);

                typeDeclaration = DeclarationFactory::makeTypeDefDeclaration(externalMetadata.Get(), externalToken, typeName.data()).release();
                break;
            }

            default:
                ASSERT_NOT_REACHED();
            }

            _types.emplace(typeName, std::unique_ptr<TypeDeclaration>(typeDeclaration));
            return *typeDeclaration;
        }

        const Type& getOrCreateArrayType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
            std::wstring fullName{ Signature(metadata, signature).toString() };

            auto existing = _types.find(fullName);
            if (existing != _types.end()) {
                return *existing->second.get();
            }

            const Type& innerType{ MetadataReader::parseType(metadata, signature) };
            ArrayType* arrayType{ new (std::nothrow) ArrayType(&innerType) };

            _types.emplace(fullName, std::unique_ptr<ArrayType>(arrayType));
            return *arrayType;
        }

        const Type& getOrCreateReferenceType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
            std::wstring fullName{ Signature(metadata, signature).toString() };

            auto existing = _types.find(fullName);
            if (existing != _types.end()) {
                return *existing->second.get();
            }

            const Type& innerType{ MetadataReader::parseType(metadata, signature) };
            ReferenceType* referenceType{ new (std::nothrow) ReferenceType(&innerType) };

            _types.emplace(fullName, std::unique_ptr<ReferenceType>(referenceType));
            return *referenceType;
        }

        const Type& getOrCreateGenericParameterType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
            ULONG index{ CorSigUncompressData(signature) };

            auto existing = _genericParameters.find(index);
            if (existing != _genericParameters.end()) {
                return *existing->second.get();
            }

            GenericParameterType* genericParameterType{ new (std::nothrow) GenericParameterType(index) };

            _genericParameters.emplace(index, std::unique_ptr<GenericParameterType>(genericParameterType));
            return *genericParameterType;
        }

        const Type& getOrCreateGenericInstanceType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
            std::wstring fullName{ Signature(metadata, signature).toString() };

            auto existing = _types.find(fullName);
            if (existing != _types.end()) {
                return *existing->second.get();
            }

            CorElementType type1{ CorSigUncompressElementType(signature) };
            ASSERT(type1 == ELEMENT_TYPE_CLASS);

            mdToken token{ CorSigUncompressToken(signature) };
            switch (TypeFromToken(token)) {
            case mdtTypeDef:
                break;

            case mdtTypeRef: {
                Microsoft::WRL::ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, token, externalMetadata.GetAddressOf(), &externalToken) };
                ASSERT(isResolved);

                break;
            }

            default:
                ASSERT_NOT_REACHED();
            }

            if (getBaseFullName(metadata, token) == SYSTEM_MULTICASTDELEGATE_W) {
            }
        }

    private:
        TypeCache()
            : _voidType{ ElementType::Void, L"Void" }
            , _booleanType{ ElementType::Boolean, L"Boolean" }
            , _char16Type{ ElementType::Char16, L"Char16" }
            , _int8Type{ ElementType::Int8, L"Int8" }
            , _uInt8Type{ ElementType::UInt8, L"UInt8" }
            , _int16Type{ ElementType::Int16, L"Int16" }
            , _uInt16Type{ ElementType::UInt16, L"UInt16" }
            , _int32Type{ ElementType::Int32, L"Int32" }
            , _uInt32Type{ ElementType::UInt32, L"UInt32" }
            , _int64Type{ ElementType::Int64, L"Int64" }
            , _uInt64Type{ ElementType::UInt64, L"UInt64" }
            , _singleType{ ElementType::Single, L"Single" }
            , _doubleType{ ElementType::Double, L"Double" }
            , _stringType{ ElementType::String, L"String" }
            , _objectType{ ElementType::Object, L"Object" }
            , _guidType{ ElementType::Guid, L"Guid" } {

            // TODO: Add simple types to dictionary
        }

        MAKE_NONCOPYABLE(TypeCache);

        const SimpleType _voidType;
        const SimpleType _booleanType;
        const SimpleType _char16Type;
        const SimpleType _int8Type;
        const SimpleType _uInt8Type;
        const SimpleType _int16Type;
        const SimpleType _uInt16Type;
        const SimpleType _int32Type;
        const SimpleType _uInt32Type;
        const SimpleType _int64Type;
        const SimpleType _uInt64Type;
        const SimpleType _singleType;
        const SimpleType _doubleType;
        const SimpleType _stringType;
        const SimpleType _objectType;
        const SimpleType _guidType;

        // TODO: Thread safe
        std::unordered_map<std::wstring, std::unique_ptr<const Type>> _types;

        // TODO: Move
        std::unordered_map<size_t, std::unique_ptr<const GenericParameterType>> _genericParameters;
    };
}
}
