#pragma once

#include <stdint.h>

namespace NativeScript {
namespace Metadata {

    // Keep this synced with CorElementType
    enum class ElementType : uint8_t {
        Void = 0x01,
        Boolean = 0x02,
        Char16 = 0x03,
        Int8 = 0x04,
        UInt8 = 0x05,
        Int16 = 0x06,
        UInt16 = 0x07,
        Int32 = 0x08,
        UInt32 = 0x09,
        Int64 = 0x0A,
        UInt64 = 0x0B,
        Single = 0x0C,
        Double = 0x0D,
        String = 0x0E,
        Reference = 0x10,
        GenericParameter = 0x13,
        Object = 0x1C,
        SingleDimensionedZeroBasedArray = 0x1D,

        Namespace = 0x23, // ELEMENT_TYPE_MAX + 1,
        Class,
        Interface,
        GenericInterface,
        GenericInterfaceInstance,
        Enum,
        Struct,
        Delegate,
        GenericDelegate,
        GenericDelegateInstance,
        Guid,
    };
}
}