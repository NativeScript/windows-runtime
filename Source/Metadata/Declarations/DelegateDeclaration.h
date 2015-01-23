#pragma once

#include <guiddef.h>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class DelegateDeclaration : public TypeDeclaration {
    public:
        typedef TypeDeclaration Base;

        using ParameterIterator = MethodDeclaration::ParameterIterator;

        explicit DelegateDeclaration(IMetaDataImport2*, mdTypeDef);

        virtual CLSID id() const;

        virtual IteratorRange<ParameterIterator> parameters() const;

    protected:
        explicit DelegateDeclaration(DeclarationKind, IMetaDataImport2*, mdTypeDef);

    private:
        MethodDeclaration _invokeMethod;
    };

    class GenericDelegateInstanceDeclaration final : public DelegateDeclaration {
    public:
        typedef DelegateDeclaration Base;

        explicit GenericDelegateInstanceDeclaration(IMetaDataImport2*, mdTypeDef, IMetaDataImport2*, mdTypeSpec);

        virtual std::wstring fullName() const override;

        virtual CLSID id() const override;

        virtual IteratorRange<ParameterIterator> parameters() const override;

    private:
        const Microsoft::WRL::ComPtr<IMetaDataImport2> _closedMetadata;
        mdTypeSpec _closedToken;
    };
}
}
