#pragma once

#include "Type.h"

namespace NativeScript {
namespace Metadata {

    struct SimpleType final : public Type {
        typedef Type Base;

    private:
        friend class TypeCache;

        explicit SimpleType(ElementType elementType, const wchar_t* stringRepresentation)
            : Base(elementType)
            , _stringRepresentation{ stringRepresentation } {
        }

        virtual void toStringInternal(std::wstring& result) const override {
            result += _stringRepresentation;
        }

        const wchar_t* _stringRepresentation;
    };
}
}
