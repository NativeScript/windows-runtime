#pragma once

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class FieldDeclaration : public Declaration {
public:
    typedef Declaration Base;

    explicit FieldDeclaration(IMetaDataImport2*, mdFieldDef);

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

protected:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdFieldDef _token;
};

}
}
