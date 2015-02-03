#pragma once

#include "Type.h"

namespace NativeScript {
namespace Metadata {

    struct GenericParameterType final : public Type {
        typedef Type Base;

        size_t index() const {
            return _index;
        }

    private:
        friend class TypeCache;

        explicit GenericParameterType(size_t index)
            : Base(ElementType::GenericParameter)
            , _index{ index } {
        }

        virtual void toStringInternal(std::wstring& result) const override {
            result += L"Var!";
            result += std::to_wstring(_index);
        }

        const size_t _index;
    };
}
}
