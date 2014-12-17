#pragma once

#include <vector>
#include <memory>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"
#include "PropertyDeclaration.h"
#include "IteratorRange.h"
#include "InterfaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

class ClassDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using MethodIterator = std::vector<MethodDeclaration>::const_iterator;
    using PropertyIterator = std::vector<PropertyDeclaration>::const_iterator;

    explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

    std::wstring baseFullName() const;

    // Events

    IteratorRange<MethodIterator> methods() const;

    IteratorRange<PropertyIterator> properties() const;

    std::vector<std::shared_ptr<Declaration>> findMembersWithName(const wchar_t*) const;

    std::vector<std::shared_ptr<MethodDeclaration>> findMethodsWithName(const wchar_t*) const;

private:
    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForInstanceInitializer(const MethodDeclaration&, size_t*);

    std::vector<MethodDeclaration> _methods;
    std::vector<PropertyDeclaration> _properties;
};

}
}
