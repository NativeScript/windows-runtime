#include "pch.h"
#include "MetadataReader.h"

#include "Ast.h"

#define WINDOWS_W                      L"Windows"
#define WINDOWS                         "Windows"

#define SYSTEM_ENUM_W                  L"System.Enum"
#define SYSTEM_ENUM                     "System.Enum"

#define SYSTEM_VALUETYPE_W             L"System.ValueType"
#define SYSTEM_VALUETYPE                "System.ValueType"

namespace {
template <typename T, typename U>
bool equalToAny(T&& t, U&& u) {
    return (t == u);
}

template <typename T, typename U, typename... Args>
bool equalToAny(T&& t, U&& u, Args&&... args) {
    return equalToAny(t, u) || equalToAny(t, args...);
}
}

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

// TODO: Create HCorenum wrapper

// TODO: Use decl_iterator, specific_decl_iterator, filtered_decl_iterator

shared_ptr<Declaration> MetadataReader::findByName(const wchar_t* fullName) const {
    ASSERT(fullName);

    if (*fullName == L'\0') {
        return make_shared<NamespaceDeclaration>(L"");
    }

    ComPtr<IMetaDataImport2> metadata;
    mdTypeDef token{mdTypeDefNil};

    HStringReference fullNameRef{fullName};
    HRESULT getMetadataFileResult{RoGetMetaDataFile(fullNameRef.Get(), nullptr, nullptr, metadata.GetAddressOf(), &token)};

    if (FAILED(getMetadataFileResult)) {
        if (equalToAny(getMetadataFileResult, E_INVALIDARG, RO_E_METADATA_INVALID_TYPE_FORMAT, RO_E_METADATA_NAME_NOT_FOUND, RO_E_INVALID_METADATA_FILE)) {
            // TODO: Check generics by appending `1, `2 ...
            return nullptr;
        }

        if (getMetadataFileResult == RO_E_METADATA_NAME_IS_NAMESPACE) {
            // TODO
            // RoResolveNamespace gives incomplete results.
            // The search for the "Windows" namespace on Windows Phone 8.1 fails both on a device and on an emulator with corrupted metadata error.

            return make_shared<NamespaceDeclaration>(fullName);
        }
    }

    ASSERT_SUCCESS(getMetadataFileResult);

    DWORD flags{0};
    mdToken parentToken{mdTokenNil};
    ASSERT_SUCCESS(metadata->GetTypeDefProps(token, nullptr, 0, nullptr, &flags, &parentToken));

    if (!IsTdPublic(flags) || IsTdSpecialName(flags)) {
        // TODO: Check deprecated?, obsolete

        return nullptr;
    }

    if (IsTdClass(flags)) {
        identifier parentName;

        switch (TypeFromToken(parentToken)) {
            case mdtTypeDef: {
                ASSERT_SUCCESS(metadata->GetTypeDefProps(parentToken, parentName.data(), parentName.size(), nullptr, nullptr, nullptr));
                break;
            }

            case mdtTypeRef: {
                ASSERT_SUCCESS(metadata->GetTypeRefProps(parentToken, nullptr, parentName.data(), parentName.size(), nullptr));
                break;
            }

            case mdtTypeSpec: {
                // TODO
                break;
            }

            default:
                ASSERT_NOT_REACHED();
        }

        ASSERT(parentName.data());

        if (wcscmp(parentName.data(), SYSTEM_ENUM_W) == 0) {
            return make_shared<EnumDeclaration>(metadata.Get(), token);
        }

        if (wcscmp(parentName.data(), SYSTEM_VALUETYPE_W) == 0) {
            return make_shared<StructDeclaration>(metadata.Get(), token);
        }

        return make_shared<ClassDeclaration>(metadata.Get(), token);
    }

    if (IsTdInterface(flags)) {
        return nullptr;
    }

    ASSERT_NOT_REACHED();
}

shared_ptr<Declaration> MetadataReader::findByNameInNamespace(const wchar_t* name, const wchar_t* namespaceName) const {
    ASSERT(name);
    ASSERT(namespaceName);

    wstring fullyQualifiedName;
    if (*namespaceName != L'\0') {
        fullyQualifiedName.append(namespaceName).append(L".");
    }
    fullyQualifiedName.append(name);

    return findByName(fullyQualifiedName.data());
}
}
}
