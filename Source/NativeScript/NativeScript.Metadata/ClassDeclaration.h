#pragma once

#include <vector>
#include <memory>

#include "BaseClassDeclaration.h"
#include "InterfaceDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class ClassDeclaration final : public BaseClassDeclaration {
public:
    typedef BaseClassDeclaration Base;

    explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

    static std::unique_ptr<const InterfaceDeclaration> declaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

    std::wstring baseFullName() const;

    bool isInstantiable() const;

    bool isSealed() const;

    IteratorRange<MethodIterator> initializers() const;

private:
    static std::unique_ptr<const InterfaceDeclaration> declaringInterfaceForInstanceInitializer(const MethodDeclaration&, size_t*);

    std::vector<MethodDeclaration> _initializers;
};

}
}
