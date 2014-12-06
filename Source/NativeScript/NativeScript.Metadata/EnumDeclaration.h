#pragma once

#include "TypeDeclaration.h"
#include "EnumMemberDeclaration.h"

namespace NativeScript {
namespace Metadata {

class EnumDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    class MemberIterator final : public std::iterator<std::input_iterator_tag, EnumMemberDeclaration> {
    public:
        explicit MemberIterator(IMetaDataImport2*, mdTypeDef, ULONG index);

        ~MemberIterator();

        MemberIterator(const MemberIterator&) = delete;
        MemberIterator(MemberIterator&&);

        MemberIterator& operator=(const MemberIterator&) = delete;
        MemberIterator& operator=(MemberIterator&&);

        EnumMemberDeclaration operator*();
        MemberIterator& operator++();

        friend bool operator==(const MemberIterator&, const MemberIterator&);
        friend bool operator!=(const MemberIterator&, const MemberIterator&);

    private:
        Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
        mdTypeDef _token;

        ULONG _currentIndex;
        HCORENUM _enumerator;
    };

    explicit EnumDeclaration(IMetaDataImport2*, mdTypeDef);

    size_t size() const;

    MemberIterator begin() const;
    MemberIterator end() const;
};

}
}
