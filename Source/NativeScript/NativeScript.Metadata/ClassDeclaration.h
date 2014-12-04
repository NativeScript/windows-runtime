#pragma once

#include <vector>

#include "MetadataDeclaration.h"
#include "MethodDeclaration.h"
#include "PropertyDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class ClassDeclaration final : public MetadataDeclaration {
public:
    typedef MetadataDeclaration Base;

    using MethodIterator = std::vector<MethodDeclaration>::const_iterator;
    using PropertyIterator = std::vector<PropertyDeclaration>::const_iterator;

    explicit ClassDeclaration(Microsoft::WRL::ComPtr<IMetaDataImport2>, mdTypeDef);

    std::wstring baseFullName() const;

    // ImplementedIntefaces
    // Constructors
    // Events

    IteratorRange<MethodIterator> methods() const;
    IteratorRange<PropertyIterator> properties() const;

private:
    std::vector<MethodDeclaration> _methods;
    std::vector<PropertyDeclaration> _properties;
};

}
}
