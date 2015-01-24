#pragma once

#include <vector>
#include <memory>

#include "BaseClassDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class ClassDeclaration final : public BaseClassDeclaration {
    public:
        typedef BaseClassDeclaration Base;

        explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

        std::wstring baseFullName() const;

        const InterfaceDeclaration& defaultInterface() const;

        bool isInstantiable() const;

        bool isSealed() const;

        IteratorRange<MethodIterator> initializers() const;

    private:
        std::vector<MethodDeclaration> _initializers;
        std::shared_ptr<InterfaceDeclaration> _defaultInterface;
    };
}
}
