#pragma once

#include <vector>
#include <memory>

#include <cor.h>
#include <wrl.h>

#include "Declaration.h"
#include "ParameterDeclaration.h"
#include "Wrappers/IteratorRange.h"

namespace NativeScript {
namespace Metadata {

    class InterfaceDeclaration;

    class MethodDeclaration final : public Declaration {
    public:
        typedef Declaration Base;

        using ParameterIterator = std::vector<const ParameterDeclaration>::const_iterator;

        explicit MethodDeclaration(IMetaDataImport2*, mdMethodDef);

        virtual bool isExported() const override;

        virtual std::wstring name() const override;

        virtual std::wstring fullName() const override;

        bool isInitializer() const;

        bool isStatic() const;

        bool isSealed() const;

        const Type& returnType() const;

        IteratorRange<ParameterIterator> parameters() const;

        size_t numberOfParameters() const;

        std::wstring overloadName() const;

        bool isDefaultOverload() const;

    private:
        friend std::unique_ptr<const InterfaceDeclaration> findDeclaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

        void parseSignature();

        const Microsoft::WRL::ComPtr<IMetaDataImport2> _metadata;
        const mdMethodDef _token;

        const Type* _returnType;
        std::vector<ParameterDeclaration> _parameters;
    };
}
}
