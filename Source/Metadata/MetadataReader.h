#pragma once

#include <memory>

#include <hstring.h>
#include "Declarations/Declarations.h"
#include "TypeCache.h"

namespace NativeScript {
namespace Metadata {

    class MetadataReader final {
    public:
        explicit MetadataReader() = delete;

        static const Type* findByName(const wchar_t* fullName);
        static const Type* findByName(HSTRING fullName);

        static const Type& parseType(IMetaDataImport2*, PCCOR_SIGNATURE);

    private:
        TypeCache _typeCache;
    };
}
}
