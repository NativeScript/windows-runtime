#pragma once

#include <stdint.h>

#include "FieldDeclaration.h"

namespace NativeScript {
namespace Metadata {

    class EnumMemberDeclaration final : public FieldDeclaration {
    public:
        typedef FieldDeclaration Base;

        explicit EnumMemberDeclaration(IMetaDataImport2*, mdFieldDef);

        int64_t value() const;
    };
}
}
