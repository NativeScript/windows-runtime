#pragma once

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class TypeDeclaration : public Declaration {
public:
    typedef Declaration Base;

    virtual bool isExported() const override;

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

protected:
    explicit TypeDeclaration(DeclarationKind, IMetaDataImport2*, mdTypeDef);

    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdTypeDef _token;
};

}
}
