#pragma once

#include <vector>

#include "TypeDeclaration.h"
#include "StructFieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class StructDeclaration final : public TypeDeclaration {
    public:
        typedef TypeDeclaration Base;

        using FieldIterator = std::vector<const StructFieldDeclaration>::const_iterator;

        explicit StructDeclaration(IMetaDataImport2*, mdTypeDef);

        size_t size() const;

        FieldIterator begin() const;
        FieldIterator end() const;

    private:
        std::vector<StructFieldDeclaration> _fields;
    };
}
}
