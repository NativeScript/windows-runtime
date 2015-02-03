#include "Metadata-Prefix.h"
#include "GenericInstanceIdBuilder.h"
#include "MetadataReader.h"
#include "Signature.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    HRESULT GenericInstanceIdBuilder::locatorImpl(PCWSTR name, IRoSimpleMetaDataBuilder& builder) {
        const Type* declaration{ MetadataReader::findByName(name) };
        ASSERT(declaration);

        ElementType elementType{ declaration->elementType() };
        switch (elementType) {
        case ElementType::Class: {
            const ClassDeclaration* classDeclaration{ static_cast<const ClassDeclaration*>(declaration) };
            const InterfaceDeclaration& defaultInterface{ classDeclaration->defaultInterface() };
            IID defaultInterfaceId = defaultInterface.id();
            ASSERT_SUCCESS(builder.SetRuntimeClassSimpleDefault(name, defaultInterface.fullName().data(), &defaultInterfaceId));
            return S_OK;
        }

        case ElementType::Interface: {
            const InterfaceDeclaration* interfaceDeclaration{ static_cast<const InterfaceDeclaration*>(declaration) };
            ASSERT_SUCCESS(builder.SetWinRtInterface(interfaceDeclaration->id()));
            return S_OK;
        }

        case ElementType::GenericInterface: {
            const GenericInterfaceDeclaration* genericInterfaceDeclaration{ static_cast<const GenericInterfaceDeclaration*>(declaration) };
            ASSERT_SUCCESS(builder.SetParameterizedInterface(genericInterfaceDeclaration->id(), genericInterfaceDeclaration->numberOfGenericParameters()));
            return S_OK;
        }

        case ElementType::Enum: {
            const EnumDeclaration* enumDeclaration{ static_cast<const EnumDeclaration*>(declaration) };
            ASSERT_SUCCESS(builder.SetEnum(enumDeclaration->fullName().data(), enumDeclaration->type().toString().data()));
            return S_OK;
        }

        case ElementType::Struct: {
            const StructDeclaration* structDeclaration{ static_cast<const StructDeclaration*>(declaration) };

            vector<wstring> fieldNames;
            for (const StructFieldDeclaration& field : *structDeclaration) {
                fieldNames.push_back(field.type().toString());
            }

            vector<const wchar_t*> fieldNamesW;
            for (const wstring& fieldName : fieldNames) {
                fieldNamesW.push_back(fieldName.data());
            }

            ASSERT_SUCCESS(builder.SetStruct(structDeclaration->fullName().data(), structDeclaration->size(), fieldNamesW.data()));
            return S_OK;
        }

        case ElementType::Delegate: {
            const DelegateDeclaration* delegateDeclaration{ static_cast<const DelegateDeclaration*>(declaration) };
            ASSERT_SUCCESS(builder.SetDelegate(delegateDeclaration->id()));
            return S_OK;
        }

        case ElementType::GenericDelegate: {
            const GenericDelegateDeclaration* genericDelegateDeclaration{ static_cast<const GenericDelegateDeclaration*>(declaration) };
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
