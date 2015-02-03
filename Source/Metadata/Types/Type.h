#pragma once

#include "ElementType.h"
#include "Wrappers/Noncopyable.h"

namespace NativeScript {
namespace Metadata {

    struct Type {
        ElementType elementType() const {
            return _elementType;
        }

        virtual ~Type() = default;

        std::wstring toString() const {
            std::wstring result;
            toStringInternal(result);
            return result;
        }

        // TODO
        virtual void toStringInternal(std::wstring&) const = 0;

    protected:
        explicit Type(ElementType corType)
            : _elementType{ corType } {
        }

    private:
        MAKE_NONCOPYABLE(Type);

        ElementType _elementType;
    };
}
}
