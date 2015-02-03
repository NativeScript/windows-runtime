#include "Metadata-Prefix.h"
#include "ComHelpers.h"
#include <codecvt>

NO_RETURN void CRASH_IMPL() {
    reinterpret_cast<void (*)()>(0xDEADDEAD)();
}

namespace NativeScript {
namespace Metadata {

    using namespace std;

    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

#ifdef _DEBUG
    void DEBUG_LOG(_Printf_format_string_ const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);
        array<wchar_t, 4096> buffer;
        _vsnwprintf_s(buffer.data(), buffer.size(), _TRUNCATE, format, args);
        va_end(args);

        OutputDebugString((wstring{ buffer.data() } + L'\n').data());
    }
#endif

    wstring getStringValueFromBlob(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
        ASSERT(metadata);
        ASSERT(signature);

        // If it's null
        if (*signature == UINT8_MAX) {
            return wstring{};
        }

        // Read size and advance
        ULONG size{ CorSigUncompressData(signature) };

        // TODO
        wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
        wstring value{ converter.from_bytes(reinterpret_cast<const char*>(signature), reinterpret_cast<const char*>(signature) + size) };
        return value;
    }

    wstring getUnaryCustomAttributeStringValue(IMetaDataImport2* metadata, mdToken token, const wchar_t* attributeName) {
        ASSERT(metadata);
        ASSERT(token != mdTokenNil);
        ASSERT(attributeName);

        const uint8_t* data{ nullptr };
        HRESULT getAttributeResult{ metadata->GetCustomAttributeByName(token, attributeName, reinterpret_cast<const void**>(&data), nullptr) };

        ASSERT_SUCCESS(getAttributeResult);

        if (getAttributeResult == S_FALSE) {
            return wstring{};
        }

        return getStringValueFromBlob(metadata, data + 2);
    }

    const wchar_t* const GUID_ATTRIBUTE_W{ L"Windows.Foundation.Metadata.GuidAttribute" };

    GUID getGuidAttributeValue(IMetaDataImport2* metadata, mdToken token) {
        ASSERT(metadata);
        ASSERT(token != mdTokenNil);

        const uint8_t* data{ nullptr };
        ASSERT_SUCCESS(metadata->GetCustomAttributeByName(token, GUID_ATTRIBUTE_W, reinterpret_cast<const void**>(&data), nullptr));

        // Skip prolog
        data += 2;

        GUID guid(*reinterpret_cast<const GUID*>(data));
        return guid;
    }

    bool resolveTypeRef(IMetaDataImport2* metadata, mdTypeRef token, IMetaDataImport2** externalMetadata, mdTypeDef* externalToken) {
        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtTypeRef);
        ASSERT(token != mdTypeRefNil);
        ASSERT(externalMetadata);
        ASSERT(externalToken);

        identifier name;
        ASSERT_SUCCESS(metadata->GetTypeRefProps(token, nullptr, name.data(), name.size(), nullptr));

        HRESULT getMetadataFileResult{ RoGetMetaDataFile(HStringReference(name.data()).Get(), nullptr, nullptr, externalMetadata, externalToken) };

        return getMetadataFileResult == S_OK;
    }

    wstring getTypeName(IMetaDataImport2* metadata, mdToken token) {
        ASSERT(metadata);
        ASSERT(token != mdTokenNil);

        identifier nameData;
        ULONG nameLength{ 0 };

        switch (TypeFromToken(token)) {
        case mdtTypeDef:
            ASSERT_SUCCESS(metadata->GetTypeDefProps(token, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr));
            break;

        case mdtTypeRef:
            ASSERT_SUCCESS(metadata->GetTypeRefProps(token, nullptr, nameData.data(), nameData.size(), &nameLength));
            break;

        default:
            ASSERT_NOT_REACHED();
        }

        wstring result{ nameData.data(), nameLength - 1 };
        return result;
    }

    wstring getBaseFullName(IMetaDataImport2* metadata, mdTypeDef token) {
        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtTypeDef);
        ASSERT(token != mdTypeDefNil);

        mdToken parentToken{ mdTokenNil };
        ASSERT_SUCCESS(metadata->GetTypeDefProps(token, nullptr, 0, nullptr, nullptr, &parentToken));

        identifier parentName;
        ULONG nameLength{ 0 };

        switch (TypeFromToken(parentToken)) {
        case mdtTypeDef: {
            ASSERT_SUCCESS(metadata->GetTypeDefProps(parentToken, parentName.data(), parentName.size(), &nameLength, nullptr, nullptr));
            break;
        }

        case mdtTypeRef: {
            ASSERT_SUCCESS(metadata->GetTypeRefProps(parentToken, nullptr, parentName.data(), parentName.size(), &nameLength));
            break;
        }

        default:
            ASSERT_NOT_REACHED();
        }

        wstring result{ parentName.data(), nameLength - 1 };
        return result;
    }

    PCCOR_SIGNATURE getTypeSpecSignature(IMetaDataImport2* metadata, mdTypeSpec token) {
        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtTypeSpec);
        ASSERT(token != mdTypeSpecNil);

        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };
        ASSERT_SUCCESS(metadata->GetTypeSpecFromToken(token, &signature, &signatureSize));
        return signature;
    }
}
}
