#pragma once

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

    class ParameterDeclaration final : public Declaration {
    public:
        typedef Declaration Base;

        explicit ParameterDeclaration(IMetaDataImport2*, mdParamDef, PCCOR_SIGNATURE);

        virtual std::wstring name() const override;

        virtual std::wstring fullName() const override;

        bool isOut() const;

        PCCOR_SIGNATURE type() const;

    private:
        const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
        const mdParamDef _token;
        const PCCOR_SIGNATURE _type;
    };
}
}
