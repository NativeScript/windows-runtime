#pragma once

#include <string>
#include <cor.h>

namespace NativeScript {
namespace Metadata {

    class Signature final {
    public:
        explicit Signature(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature);

        std::wstring toString();

    private:
        IMetaDataImport2* _metadata;
        const PCCOR_SIGNATURE _signature;
    };

    inline PCCOR_SIGNATURE consumeType(PCCOR_SIGNATURE& signature) {
        PCCOR_SIGNATURE start{ signature };

        CorElementType elementType{ CorSigUncompressElementType(signature) };
        switch (elementType) {
        case ELEMENT_TYPE_VOID:
        case ELEMENT_TYPE_BOOLEAN:
        case ELEMENT_TYPE_CHAR:
        case ELEMENT_TYPE_I1:
        case ELEMENT_TYPE_U1:
        case ELEMENT_TYPE_I2:
        case ELEMENT_TYPE_U2:
        case ELEMENT_TYPE_I4:
        case ELEMENT_TYPE_U4:
        case ELEMENT_TYPE_I8:
        case ELEMENT_TYPE_U8:
        case ELEMENT_TYPE_R4:
        case ELEMENT_TYPE_R8:
        case ELEMENT_TYPE_STRING:
            return start;

        case ELEMENT_TYPE_VALUETYPE:
            CorSigUncompressToken(signature);
            return start;

        case ELEMENT_TYPE_CLASS:
            CorSigUncompressToken(signature);
            return start;

        case ELEMENT_TYPE_OBJECT:
            return start;

        case ELEMENT_TYPE_SZARRAY:
            consumeType(signature);
            return start;

        case ELEMENT_TYPE_VAR:
            CorSigUncompressData(signature);
            return start;

        case ELEMENT_TYPE_GENERICINST: {
            CorSigUncompressElementType(signature);
            CorSigUncompressToken(signature);

            ULONG genericArgumentsCount{ CorSigUncompressData(signature) };
            for (size_t i = 0; i < genericArgumentsCount; ++i) {
                consumeType(signature);
            }

            return start;
        }

        case ELEMENT_TYPE_BYREF:
            consumeType(signature);
            return start;

        default:
            ASSERT_NOT_REACHED();
        }
    }
}
}
