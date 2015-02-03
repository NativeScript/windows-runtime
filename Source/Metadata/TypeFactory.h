#pragma once

#include <memory>
#include <unordered_map>

#include <cor.h>
#include "Metadata-Prefix.h"

#include "Declarations/Declarations.h"
#include "Types/Types.h"
#include "DeclarationFactory.h"

namespace NativeScript {
namespace Metadata {
    class TypeCache final {
    public:
        static TypeCache& instance() {
            static TypeCache instance;
            return instance;
        }

        const Type& consumeSignature(IMetaDataImport2* metadata, PCCOR_SIGNATURE& signature) {
            CorElementType elementType{ CorSigUncompressElementType(signature) };
            switch (elementType) {
            case ELEMENT_TYPE_VOID:
                return _voidType;

            case ELEMENT_TYPE_BOOLEAN:
                return _booleanType;

            case ELEMENT_TYPE_CHAR:
                return _char16Type;

            case ELEMENT_TYPE_I1:
                return _int8Type;

            case ELEMENT_TYPE_U1:
                return _uInt8Type;

            case ELEMENT_TYPE_I2:
                return _int16Type;

            case ELEMENT_TYPE_U2:
                return _uInt16Type;

            case ELEMENT_TYPE_I4:
                return _int32Type;

            case ELEMENT_TYPE_U4:
                return _uInt32Type;

            case ELEMENT_TYPE_I8:
                return _int64Type;

            case ELEMENT_TYPE_U8:
                return _uInt64Type;

            case ELEMENT_TYPE_R4:
                return _singleType;

            case ELEMENT_TYPE_R8:
                return _doubleType;

            case ELEMENT_TYPE_STRING:
                return _stringType;

            // TODO: Remove wstring
            case ELEMENT_TYPE_VALUETYPE: {
                mdToken token{ CorSigUncompressToken(signature) };
                std::wstring className{ getTypeName(metadata, token) };

                if (className == L"System.Guid") {
                    return _guidType;
                }

                auto existing = _valueCache.find(className);
                if (existing != _valueCache.end()) {
                    return *existing->second.get();
                }

                Type* newType = static_cast<Type*>(MetadataReader::instance().findByName(className.data()));
                _valueCache.insert({ className, std::unique_ptr<Type>{ newType } });
                return *newType;
            }

            // TODO: Remove wstring
            case ELEMENT_TYPE_CLASS: {
                mdToken token{ CorSigUncompressToken(signature) };
                std::wstring className{ getTypeName(metadata, token) };
            }

            case ELEMENT_TYPE_OBJECT:
                return _objectType;

            case ELEMENT_TYPE_SZARRAY:

            case ELEMENT_TYPE_VAR:

            case ELEMENT_TYPE_GENERICINST: {
            }

            case ELEMENT_TYPE_BYREF:

            default:
                ASSERT_NOT_REACHED();
            }
        }

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
            auto existing = _namespaces.find(fullName);
            if (existing->second != _namespaces.end) {
                return *existing->second.get();
            }

            std::unique_ptr<NamespaceDeclaration> namespaceDeclaration{ DeclarationFactory::makeNamespaceDeclaration(fullName) };
            _namespaces.emplace(namespaceDeclaration->fullName(), move(namespaceDeclaration));

            return *namespaceDeclaration.get();
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
        }

        MAKE_NONCOPYABLE(TypeFactory);

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

        std::unordered_map<std::wstring, std::unique_ptr<NamespaceDeclaration>> _namespaces;
    };
}
}
