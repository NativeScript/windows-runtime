#pragma once

#include <vector>
#include <memory>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"
#include "PropertyDeclaration.h"
#include "EventDeclaration.h"
#include "InterfaceDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

enum class ClassType {
    Uninstantiable = 0x00,
    Instantiable = 0x01,
    Subclassable = 0x02
};

class ClassDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using MethodIterator = std::vector<const MethodDeclaration>::const_iterator;
    using PropertyIterator = std::vector<const PropertyDeclaration>::const_iterator;
    using EventIterator = std::vector<const EventDeclaration>::const_iterator;

    explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

    std::wstring baseFullName() const;

    ClassType classType();

    IteratorRange<MethodIterator> initializers() const;

    IteratorRange<MethodIterator> methods() const;

    IteratorRange<PropertyIterator> properties() const;

    IteratorRange<EventIterator> events() const;

    std::vector<std::unique_ptr<Declaration>> findMembersWithName(const wchar_t*) const;

    std::vector<MethodDeclaration> findMethodsWithName(const wchar_t*) const;

private:
    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForInstanceInitializer(const MethodDeclaration&, size_t*);

    std::vector<MethodDeclaration> _initializers;
    std::vector<MethodDeclaration> _methods;
    std::vector<PropertyDeclaration> _properties;
    std::vector<EventDeclaration> _events;
};

}
}
