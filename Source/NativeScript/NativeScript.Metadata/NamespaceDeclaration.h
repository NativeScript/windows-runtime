#pragma once

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class NamespaceDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit NamespaceDeclaration(const wchar_t* fullName);

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

private:
    std::wstring _fullName;
};

}
}
