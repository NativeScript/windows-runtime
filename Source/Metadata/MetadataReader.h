#pragma once

#include <memory>

#include <hstring.h>
#include "Declarations/Declarations.h"

namespace NativeScript {
namespace Metadata {

    class MetadataReader {
    public:
        std::shared_ptr<Declaration> findByName(const wchar_t* fullName) const;
        std::shared_ptr<Declaration> findByName(HSTRING fullName) const;
    };
}
}
