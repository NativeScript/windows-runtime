#pragma once

#include <vector>

#include "Declaration.h"
#include "Types/Type.h"
#include "Wrappers/IteratorRange.h"

namespace NativeScript {
namespace Metadata {

    class NamespaceDeclaration final : public Type, public Declaration {
    public:
        using ChildrenIterator = std::vector<const std::wstring>::const_iterator;

        explicit NamespaceDeclaration(const wchar_t* fullName);

        virtual std::wstring name() const override;

        virtual std::wstring fullName() const override;

        // Incomplete
        IteratorRange<ChildrenIterator> children();

        virtual void toStringInternal(std::wstring&) const override;

    private:
        std::wstring _fullName;
        std::vector<std::wstring> _children;
    };
}
}
