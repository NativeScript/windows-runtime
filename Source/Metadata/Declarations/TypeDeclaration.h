#pragma once

#include <cor.h>
#include <wrl.h>

#include "Types/Type.h"
#include "Declarations/Declaration.h"

namespace NativeScript {
namespace Metadata {

    class TypeDeclaration : public Type, public Declaration {
    public:
        virtual bool isExported() const override;

        virtual std::wstring name() const override;

        virtual std::wstring fullName() const override;

        virtual void toStringInternal(std::wstring&) const override;

    protected:
        explicit TypeDeclaration(ElementType, IMetaDataImport2*, mdTypeDef);

        // TODO: Friend typeFactory and private constructors in all leafs

        const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
        const mdTypeDef _token;
    };
}
}
