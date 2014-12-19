#include "pch.h"
#include "ComHelpers.h"
#include <codecvt>

NO_RETURN void CRASH() {
    CRASH(E_FAIL);
}

NO_RETURN void CRASH(HRESULT hresult) {
#if _DEBUG
    if (IsDebuggerPresent()) {
        __debugbreak();
    }
#endif

    reinterpret_cast<void(*)()>(0xDEADDEAD)();
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

    OutputDebugString((wstring{buffer.data()} + L'\n').data());
}

wstring getStringValueFromBlob(IMetaDataImport2* metadata, mdToken token, PCCOR_SIGNATURE signature) {
    // If it's null
    if (*signature == UINT8_MAX) {
        return wstring{};
    }

    // Read size and advance
    ULONG size{CorSigUncompressData(signature)};

    // TODO
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring value{converter.from_bytes(reinterpret_cast<const char*>(signature), reinterpret_cast<const char*>(signature) + size)};
    return value;
}

wstring getUnaryCustomAttributeStringValue(IMetaDataImport2* metadata, mdToken token, const wchar_t* attributeName) {
    const uint8_t* data{nullptr};
    HRESULT getAttributeResult{metadata->GetCustomAttributeByName(token, attributeName, reinterpret_cast<const void**>(&data), nullptr)};

    ASSERT_SUCCESS(getAttributeResult);

    if (getAttributeResult == S_FALSE) {
        return wstring{};
    }

    return getStringValueFromBlob(metadata, token, data + 2);
}

const wchar_t* const GUID_ATTRIBUTE_W{L"Windows.Foundation.Metadata.GuidAttribute"};

GUID getGuidAttributeValue(IMetaDataImport2* metadata, mdToken token) {
    const uint8_t* data{nullptr};
    ASSERT_SUCCESS(metadata->GetCustomAttributeByName(token, GUID_ATTRIBUTE_W, reinterpret_cast<const void**>(&data), nullptr));

    // Skip prolog
    data += 2;

    GUID guid(*reinterpret_cast<const GUID*>(data));
    return guid;
}

bool resolveTypeRef(IMetaDataImport2* metadata, mdTypeRef token, IMetaDataImport2** externalMetadata, mdTypeDef* externalToken) {
    ASSERT(metadata);
    ASSERT(TypeFromToken(token) == mdtTypeRef);
    ASSERT(externalMetadata);
    ASSERT(externalToken);

    identifier name;
    ASSERT_SUCCESS(metadata->GetTypeRefProps(token, nullptr, name.data(), name.size(), nullptr));

    HRESULT getMetadataFileResult{RoGetMetaDataFile(HStringReference(name.data()).Get(), nullptr, nullptr, externalMetadata, externalToken)};

    return getMetadataFileResult == S_OK;
}
#endif

}
}
