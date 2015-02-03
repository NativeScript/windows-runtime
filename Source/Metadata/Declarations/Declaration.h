#pragma once

#include <string>

namespace NativeScript {
namespace Metadata {

    class Declaration {
    public:
        virtual ~Declaration() = default;

        virtual bool isExported() const;

        /// Specifies the simple name (e.g., "String" rather than "System.String") of a given type.
        virtual std::wstring name() const = 0;

        /// Specifies the fully-qualified name of a given type.
        /// For generic types, this includes the spelling of generic parameter names.
        virtual std::wstring fullName() const = 0;

    protected:
        explicit Declaration() = default;
    };
}
}
