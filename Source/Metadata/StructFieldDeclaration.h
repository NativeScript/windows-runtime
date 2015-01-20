#pragma once

#include "FieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class StructFieldDeclaration final : public FieldDeclaration {
    public:
        typedef FieldDeclaration Base;

        explicit StructFieldDeclaration(IMetaDataImport2*, mdFieldDef);

        PCCOR_SIGNATURE signature() const;
    };
}
}
