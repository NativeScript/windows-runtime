#pragma once

#include <guiddef.h>

#include "TypeDeclaration.h"

namespace NativeScript {
namespace Metadata {

class InterfaceDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    explicit InterfaceDeclaration(IMetaDataImport2*, mdTypeDef);

    IID id() const;
};

}
}
