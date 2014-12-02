#pragma once

#include <string>
#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class MetadataDeclaration : public Declaration {
public:
    typedef Declaration Base;

    virtual std::wstring name() const override;

protected:
    explicit MetadataDeclaration(Microsoft::WRL::ComPtr<IMetaDataImport2>, mdTypeDef);

    Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    mdTypeDef _token;
};

}
}