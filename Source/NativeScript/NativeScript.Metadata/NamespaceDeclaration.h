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

    // TODO
    // RoResolveNamespace gives incomplete results.
    // The search for the "Windows" namespace on Windows Phone 8.1 fails both on a device and on an emulator with corrupted metadata error.

private:
    std::wstring _fullName;
};

}
}
