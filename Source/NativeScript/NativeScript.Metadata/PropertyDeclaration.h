#pragma once

#include <memory>
#include <cor.h>
#include <wrl.h>

#include "Declaration.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

class PropertyDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit PropertyDeclaration(IMetaDataImport2*, mdProperty);

    virtual bool isExported() const override;

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

    bool isStatic() const;

    bool isOverridable() const;

    std::unique_ptr<MethodDeclaration> getter() const;

    std::unique_ptr<MethodDeclaration> setter() const;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdProperty _token;
};

}
}
