#pragma once

#include <cor.h>
#include <wrl.h>

#include "MetadataDeclaration.h"
#include "EnumMemberDeclaration.h"

namespace NativeScript {
namespace Metadata {
namespace Ast {

class EnumDeclaration final : public MetadataDeclaration {
public:
    typedef MetadataDeclaration Base;

    class MemberIterator final : public std::iterator<std::forward_iterator_tag, EnumMemberDeclaration> {
    public:
        explicit MemberIterator(const Microsoft::WRL::ComPtr<IMetaDataImport2>&, const mdTypeDef, ULONG index);

        ~MemberIterator();

        EnumMemberDeclaration operator*() const;

        MemberIterator& operator++();
        MemberIterator operator++(int);

        friend bool operator==(const MemberIterator&, const MemberIterator&);
        friend bool operator!=(const MemberIterator&, const MemberIterator&);

    private:
        const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
        const mdTypeDef _token;

        ULONG _currentIndex;
        mutable HCORENUM _enumerator;
    };

    explicit EnumDeclaration(const Microsoft::WRL::ComPtr<IMetaDataImport2>&, const mdTypeDef);

    size_t size() const;
    MemberIterator begin() const;
    MemberIterator end() const;
};

bool operator==(const EnumDeclaration::MemberIterator&, const EnumDeclaration::MemberIterator&);
bool operator!=(const EnumDeclaration::MemberIterator&, const EnumDeclaration::MemberIterator&);

}
}
}