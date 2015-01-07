#pragma once

#include <memory>
#include <cor.h>
#include <wrl.h>

#include "Declaration.h"
#include "MethodDeclaration.h"
#include "DelegateDeclaration.h"

namespace NativeScript {
namespace Metadata {

class EventDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit EventDeclaration(IMetaDataImport2*, mdEvent);

    virtual bool isExported() const override;

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

    bool isStatic() const;

    bool isSealed() const;

    const DelegateDeclaration* type() const;

    const MethodDeclaration& addMethod() const;

    const MethodDeclaration& removeMethod() const;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdEvent _token;

    std::shared_ptr<DelegateDeclaration> _type;
    MethodDeclaration _addMethod;
    MethodDeclaration _removeMethod;
};

}
}
