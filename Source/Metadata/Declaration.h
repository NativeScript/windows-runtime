#pragma once

#include <string>

namespace NativeScript {
namespace Metadata {

enum class DeclarationKind : uint8_t {
    Namespace,
    Class,
    Interface,
    GenericInterfaceInstance,
    Enum,
    EnumMember,
    Struct,
    StructField,
    Delegate,
    GenericDelegateInstance,
    Event,
    Property,
    Method,
    Parameter,
};

class Declaration {
public:
    virtual ~Declaration() = default;

    virtual bool isExported() const;

    /// Specifies the simple name (e.g., "String" rather than "System.String") of a given type.
    virtual std::wstring name() const = 0;

    /// Specifies the fully-qualified name of a given type.
    /// For generic types, this includes the spelling of generic parameter names.
    virtual std::wstring fullName() const = 0;

    DeclarationKind kind() const;

protected:
    explicit Declaration(DeclarationKind);

private:
    DeclarationKind _kind;
};

}
}
