#pragma once

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

class MethodDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    explicit MethodDeclaration(Microsoft::WRL::ComPtr<IMetaDataImport2>, mdMethodDef);

    virtual bool isExported() const override;

    bool isStatic() const;

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdMethodDef _token;
};

}
}
