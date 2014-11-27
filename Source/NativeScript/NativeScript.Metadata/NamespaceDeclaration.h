#pragma once

#include <string>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {
namespace Ast {

class NamespaceDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit NamespaceDeclaration(const wchar_t* name);

    virtual std::wstring name() const override;

private:
    std::wstring _name;
};

}
}
}