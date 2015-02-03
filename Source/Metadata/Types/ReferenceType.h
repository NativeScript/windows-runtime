#pragma once

namespace NativeScript {
namespace Metadata {

    struct ReferenceType final : public Type {
        typedef Type Base;

        const Type* innerType() const {
            return _innerType;
        }

        virtual void toStringInternal(std::wstring& result) const override {
            result += L"ByRef ";
            _innerType->toStringInternal(result);
        }

    private:
        friend class TypeCache;

        explicit ReferenceType(const Type* innerType)
            : Base(ElementType::Reference)
            , _innerType{ innerType } {
            ASSERT(innerType);
        }

        const Type* const _innerType;
    };
}
}