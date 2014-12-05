#pragma once

#include <vector>
#include <memory>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"
#include "PropertyDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class ClassDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using MethodIterator = std::vector<MethodDeclaration>::const_iterator;
    using PropertyIterator = std::vector<PropertyDeclaration>::const_iterator;

    explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

    std::wstring baseFullName() const;

    // ImplementedIntefaces
    // Constructors
    // Events

    IteratorRange<MethodIterator> methods() const;
    IteratorRange<PropertyIterator> properties() const;

    std::vector<std::shared_ptr<Declaration>> findMembersWithName(const wchar_t*) const;

private:
    std::vector<MethodDeclaration> _methods;
    std::vector<PropertyDeclaration> _properties;
};

}
}
