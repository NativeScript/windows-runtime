#pragma once

#include <string>
#include <cor.h>

namespace NativeScript {
namespace Metadata {

    class Signature {
    public:
        static PCCOR_SIGNATURE consumeType(PCCOR_SIGNATURE&);

        static std::wstring toString(IMetaDataImport2*, PCCOR_SIGNATURE);

    private:
        static void toString(IMetaDataImport2*, PCCOR_SIGNATURE, std::wstring&);
    };
}
}
