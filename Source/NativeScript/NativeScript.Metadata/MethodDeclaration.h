#pragma once

#include <vector>

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"
#include "ParameterDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class MethodDeclaration final : public Declaration {
public:
    typedef Declaration Base;

    using ParameterIterator = std::vector<ParameterDeclaration>::const_iterator;

    explicit MethodDeclaration(IMetaDataImport2*, mdMethodDef);

    virtual bool isExported() const override;

    virtual std::wstring name() const override;

    virtual std::wstring fullName() const override;

    bool isStatic() const;

    IteratorRange<ParameterIterator> parameters() const;

    std::wstring overloadName() const;

    bool isDefaultOverload() const;

private:
    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdMethodDef _token;

    std::vector<ParameterDeclaration> _parameters;
};

}
}
