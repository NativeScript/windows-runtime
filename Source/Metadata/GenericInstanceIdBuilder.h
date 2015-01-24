#pragma once

#include <guiddef.h>
#include "Declarations/Declaration.h"
#include "Roparameterizediid.h"

namespace NativeScript {
namespace Metadata {

    // TODO
    class GenericInstanceIdBuilder final {
    public:
        GenericInstanceIdBuilder() = delete;

        static GUID generateId(const Declaration&);

    private:
        static HRESULT locatorImpl(PCWSTR name, IRoSimpleMetaDataBuilder& builder);
    };
}
}
