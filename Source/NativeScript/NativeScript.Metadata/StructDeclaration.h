#pragma once

#include <vector>

#include "TypeDeclaration.h"
#include "FieldDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class StructDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using FieldIterator = std::vector<const FieldDeclaration>::const_iterator;

    explicit StructDeclaration(IMetaDataImport2*, mdTypeDef);

    size_t size() const;

    FieldIterator begin() const;
    FieldIterator end() const;

private:
    std::vector<FieldDeclaration> _fields;
};

}
}
