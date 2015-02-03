#pragma once

#include "FieldDeclaration.h"
#include "Types/Type.h"

namespace NativeScript {
namespace Metadata {

    class StructFieldDeclaration final : public FieldDeclaration {
    public:
        typedef FieldDeclaration Base;

        explicit StructFieldDeclaration(IMetaDataImport2*, mdFieldDef);

        const Type& type() const;
    };
}
}
