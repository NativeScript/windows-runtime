#pragma once

#include <stdint.h>
#include <string>
#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class EnumMemberDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit EnumMemberDeclaration(Microsoft::WRL::ComPtr<IMetaDataImport2>, mdFieldDef);

    virtual std::wstring name() const override;

    int64_t value() const;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdFieldDef _token;
};

}
}