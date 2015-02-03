#pragma once

#include "Type.h"

namespace NativeScript {
namespace Metadata {

    struct ArrayType final : public Type {
        typedef Type Base;

        const Type* innerType() const {
            return _innerType;
        }

    private:
        friend class TypeCache;

        explicit ArrayType(const Type* innerType)
            : Base(ElementType::SingleDimensionedZeroBasedArray)
            , _innerType{ innerType } {
            ASSERT(innerType);
        }

        virtual void toStringInternal(std::wstring& result) const override {
            _innerType->toStringInternal(result);
            result += L"[]";
        }

        const Type* const _innerType;
    };
}
}
