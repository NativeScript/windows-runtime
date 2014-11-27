#pragma once

#include <string>
#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {
namespace Ast {

class MetadataDeclaration : public Declaration {
public:
    typedef Declaration Base;

    virtual std::wstring name() const override;

protected:
    explicit MetadataDeclaration(const Microsoft::WRL::ComPtr<IMetaDataImport2>&, const mdTypeDef);

    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdTypeDef _token;
};

}
}
}