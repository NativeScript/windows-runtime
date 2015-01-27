#include "Metadata-Prefix.h"
#include "GenericInstanceIdBuilder.h"
#include "MetadataReader.h"
#include "Signature.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    static MetadataReader metadataReader;

    HRESULT GenericInstanceIdBuilder::locatorImpl(PCWSTR name, IRoSimpleMetaDataBuilder& builder) {
        shared_ptr<Declaration> declaration{ metadataReader.findByName(name) };
        ASSERT(declaration);

        DeclarationKind kind{ declaration->kind() };
        switch (kind) {
        case DeclarationKind::Class: {
            ClassDeclaration* classDeclaration{ static_cast<ClassDeclaration*>(declaration.get()) };
            const InterfaceDeclaration& defaultInterface{ classDeclaration->defaultInterface() };
            IID defaultInterfaceId = defaultInterface.id();
            ASSERT_SUCCESS(builder.SetRuntimeClassSimpleDefault(name, defaultInterface.fullName().data(), &defaultInterfaceId));
            return S_OK;
        }

        case DeclarationKind::Interface: {
            InterfaceDeclaration* interfaceDeclaration{ static_cast<InterfaceDeclaration*>(declaration.get()) };
            ASSERT_SUCCESS(builder.SetWinRtInterface(interfaceDeclaration->id()));
            return S_OK;
        }

        case DeclarationKind::GenericInterface: {
            GenericInterfaceDeclaration* genericInterfaceDeclaration{ static_cast<GenericInterfaceDeclaration*>(declaration.get()) };
            ASSERT_SUCCESS(builder.SetParameterizedInterface(genericInterfaceDeclaration->id(), genericInterfaceDeclaration->numberOfGenericParameters()));
            return S_OK;
        }

        case DeclarationKind::Enum: {
            EnumDeclaration* enumDeclaration{ static_cast<EnumDeclaration*>(declaration.get()) };
            ASSERT_SUCCESS(builder.SetEnum(enumDeclaration->fullName().data(), Signature::toString(nullptr, enumDeclaration->type()).data()));
            return S_OK;
        }

        case DeclarationKind::Struct: {
            StructDeclaration* structDeclaration{ static_cast<StructDeclaration*>(declaration.get()) };

            vector<wstring> fieldNames;
            for (const StructFieldDeclaration& field : *structDeclaration) {
                fieldNames.push_back(Signature::toString(field._metadata.Get(), field.type()));
            }

            vector<const wchar_t*> fieldNamesW;
            for (const wstring& fieldName : fieldNames) {
                fieldNamesW.push_back(fieldName.data());
            }

            ASSERT_SUCCESS(builder.SetStruct(structDeclaration->fullName().data(), structDeclaration->size(), fieldNamesW.data()));
            return S_OK;
        }

        case DeclarationKind::Delegate: {
            DelegateDeclaration* delegateDeclaration{ static_cast<DelegateDeclaration*>(declaration.get()) };
            ASSERT_SUCCESS(builder.SetDelegate(delegateDeclaration->id()));
            return S_OK;
        }

        case DeclarationKind::GenericDelegate: {
            GenericDelegateDeclaration* genericDelegateDeclaration{ static_cast<GenericDelegateDeclaration*>(declaration.get()) };
            ASSERT_SUCCESS(builder.SetParameterizedDelegate(genericDelegateDeclaration->id(), genericDelegateDeclaration->numberOfGenericParameters()));
            return S_OK;
        }

        default:
            ASSERT_NOT_REACHED();
        }
    }

    GUID GenericInstanceIdBuilder::generateId(const Declaration& declaration) {
        wstring declarationFullName{ declaration.fullName() };

        HSTRING* nameParts{ nullptr };
        DWORD namePartsCount{ 0 };
        ASSERT_SUCCESS(RoParseTypeName(HStringReference(declarationFullName.data()).Get(), &namePartsCount, &nameParts));

        array<const wchar_t*, 128> namePartsW;
        ASSERT(namePartsCount < namePartsW.size());

        for (size_t i = 0; i < namePartsCount; ++i) {
            namePartsW[i] = WindowsGetStringRawBuffer(nameParts[i], nullptr);
        }

        GUID guid;
        ASSERT_SUCCESS(RoGetParameterizedTypeInstanceIID(namePartsCount, namePartsW.data(), Ro::Locator(&locatorImpl), &guid, nullptr));

        for (size_t i = 0; i < namePartsCount; ++i) {
            ASSERT_SUCCESS(WindowsDeleteString(nameParts[i]));
        }

        CoTaskMemFree(nameParts);

        return guid;
    }
}
}
