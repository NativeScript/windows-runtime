#pragma once

#include <vector>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"
#include "PropertyDeclaration.h"
#include "EventDeclaration.h"

namespace NativeScript {
namespace Metadata {

class BaseClassDeclaration : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using MethodIterator = std::vector<const MethodDeclaration>::const_iterator;

    using PropertyIterator = std::vector<const PropertyDeclaration>::const_iterator;

    using EventIterator = std::vector<const EventDeclaration>::const_iterator;

    IteratorRange<MethodIterator> methods() const;

    IteratorRange<PropertyIterator> properties() const;

    IteratorRange<EventIterator> events() const;

    std::vector<std::unique_ptr<Declaration>> findMembersWithName(const wchar_t*) const;

    std::vector<MethodDeclaration> findMethodsWithName(const wchar_t*) const;

protected:
    explicit BaseClassDeclaration(IMetaDataImport2*, mdTypeDef);

private:
    std::vector<MethodDeclaration> _methods;

    std::vector<PropertyDeclaration> _properties;

    std::vector<EventDeclaration> _events;
};

}
}
