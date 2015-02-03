#include "Metadata-Prefix.h"
#include "EnumDeclaration.h"
#include "TypeCache.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    EnumDeclaration::MemberIterator::MemberIterator(IMetaDataImport2* metadata, mdTypeDef token, ULONG index)
        : _metadata{ metadata }
        , _token{ token }
        , _currentIndex{ index }
        , _enumerator{ nullptr } {

        ASSERT_SUCCESS(_metadata->EnumFields(&_enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->ResetEnum(_enumerator, _currentIndex));
    }

    EnumDeclaration::MemberIterator::~MemberIterator() {
        if (_enumerator) {
            _metadata->CloseEnum(_enumerator);
            _enumerator = nullptr;
        }
    }

    EnumDeclaration::MemberIterator::MemberIterator(MemberIterator&& other)
        : _metadata{ move(other._metadata) }
        , _token{ move(other._token) }
        , _currentIndex{ move(other._currentIndex) }
        , _enumerator{ move(other._enumerator) } {

        other._enumerator = nullptr;
    }

    EnumDeclaration::MemberIterator& EnumDeclaration::MemberIterator::operator=(MemberIterator&& other) {
        _metadata = move(other._metadata);
        _token = move(other._token);
        _currentIndex = move(other._currentIndex);
        _enumerator = move(other._enumerator);

        other._enumerator = nullptr;

        return *this;
    }

    EnumMemberDeclaration EnumDeclaration::MemberIterator::operator*() {
        mdFieldDef field{ 0 };

        ASSERT_SUCCESS(_metadata->EnumFields(&_enumerator, _token, &field, 1, nullptr));
        ++_currentIndex;

        return EnumMemberDeclaration(_metadata.Get(), field);
    }

    EnumDeclaration::MemberIterator& EnumDeclaration::MemberIterator::operator++() {
        // Pretend to move
        return *this;
    }

    EnumDeclaration::EnumDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(ElementType::Enum, metadata, token) {
    }

    const Type& EnumDeclaration::type() const {
        mdFieldDef typeField{ mdTokenNil };
        ASSERT_SUCCESS(_metadata->FindField(_token, COR_ENUM_FIELD_NAME_W, nullptr, 0, &typeField));

        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };
        ASSERT_SUCCESS(_metadata->GetFieldProps(typeField, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureSize, nullptr, nullptr, nullptr));

#if _DEBUG
        PCCOR_SIGNATURE startSignature{ signature };
#endif

        ULONG header{ CorSigUncompressData(signature) };
        ASSERT(header == IMAGE_CEE_CS_CALLCONV_FIELD);

        const Type& fieldType{ MetadataReader::parseType(_metadata.Get(), consumeType(signature)) };
        ASSERT(startSignature + signatureSize == signature);

        return fieldType;
    }

    // Skips COR_ENUM_FIELD field
    size_t EnumDeclaration::size() const {
        ULONG size{ 0 };

        HCORENUM enumerator{ nullptr };
        ASSERT_SUCCESS(_metadata->EnumFields(&enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &size));
        _metadata->CloseEnum(enumerator);

        return size - 1;
    }

    EnumDeclaration::MemberIterator EnumDeclaration::begin() const {
        return MemberIterator(_metadata.Get(), _token, 1);
    }

    EnumDeclaration::MemberIterator EnumDeclaration::end() const {
        return MemberIterator(_metadata.Get(), _token, size() + 1);
    }

    bool operator==(const EnumDeclaration::MemberIterator& left, const EnumDeclaration::MemberIterator& right) {
        return left._currentIndex == right._currentIndex;
    }

    bool operator!=(const EnumDeclaration::MemberIterator& left, const EnumDeclaration::MemberIterator& right) {
        return left._currentIndex != right._currentIndex;
    }
}
}
