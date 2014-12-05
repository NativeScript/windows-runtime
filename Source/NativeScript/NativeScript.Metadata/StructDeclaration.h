#pragma once

#include <vector>

#include "MetadataDeclaration.h"
#include "FieldDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

class StructDeclaration final : public MetadataDeclaration {
public:
    typedef MetadataDeclaration Base;

    using FieldIterator = std::vector<FieldDeclaration>::const_iterator;

    explicit StructDeclaration(Microsoft::WRL::ComPtr<IMetaDataImport2>, mdTypeDef);

    size_t size() const;

    FieldIterator begin() const;
    FieldIterator end() const;

private:
    std::vector<FieldDeclaration> _fields;
};

}
}
