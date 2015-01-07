#pragma once

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class ParameterDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit ParameterDeclaration(IMetaDataImport2*, mdParamDef);

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

    bool isOut() const;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdParamDef _token;
};

}
}
