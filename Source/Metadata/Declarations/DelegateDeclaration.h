#pragma once

#include <guiddef.h>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class DelegateDeclaration : public TypeDeclaration {
    public:
        typedef TypeDeclaration Base;

        explicit DelegateDeclaration(IMetaDataImport2*, mdTypeDef);

        virtual CLSID id() const;

        const MethodDeclaration& invokeMethod() const;

    protected:
        explicit DelegateDeclaration(DeclarationKind, IMetaDataImport2*, mdTypeDef);

    private:
        MethodDeclaration _invokeMethod;
    };

    class GenericDelegateDeclaration final : public DelegateDeclaration {
    public:
        typedef DelegateDeclaration Base;

        explicit GenericDelegateDeclaration(IMetaDataImport2*, mdTypeDef);

        size_t numberOfGenericParameters() const;
    };

    class GenericDelegateInstanceDeclaration final : public DelegateDeclaration {
    public:
        typedef DelegateDeclaration Base;

        explicit GenericDelegateInstanceDeclaration(IMetaDataImport2*, mdTypeDef, IMetaDataImport2*, mdTypeSpec);

        virtual std::wstring fullName() const override;

        virtual CLSID id() const override;

    private:
        const Microsoft::WRL::ComPtr<IMetaDataImport2> _closedMetadata;
        mdTypeSpec _closedToken;
    };
}
}
