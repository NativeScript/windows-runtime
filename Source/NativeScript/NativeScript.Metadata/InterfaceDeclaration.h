#pragma once

#include <guiddef.h>

#include "BaseClassDeclaration.h"

namespace NativeScript {
namespace Metadata {

class InterfaceDeclaration final : public BaseClassDeclaration {
public:
    typedef BaseClassDeclaration Base;

    explicit InterfaceDeclaration(IMetaDataImport2*, mdTypeDef);

    IID id() const;
};

}
}
