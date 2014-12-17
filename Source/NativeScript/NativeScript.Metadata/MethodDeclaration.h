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

    bool isInstanceInitializer() const;

    bool isStatic() const;

    IteratorRange<ParameterIterator> parameters() const;
    size_t numberOfArguments() const;

    std::wstring overloadName() const;

    bool isDefaultOverload() const;

private:
    friend class ClassDeclaration;

    const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
    const mdMethodDef _token;

    std::vector<ParameterDeclaration> _parameters;
};

}
}
