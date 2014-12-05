using System;

namespace NativeScript.Tests.Fixtures
{
    // Keep fields sorted by field name

    public struct SimpleStruct
    {
        // Static fields are compiled, but can't be linked

        public Boolean BooleanField;
        public Byte ByteField;
        public Char CharField;
        public Double DoubleField;
        public Int16 Int16Field;
        public Int32 Int32Field;
        public Int64 Int64Field;
        public SimpleEnum SimpleEnumField;
        public Single SingleField;
        public String StringField;
        public UInt16 UInt16Field;
        public UInt32 UInt32Field;
        public UInt64 UInt64Field;
    }

    // TODO: Nested struct
    // TODO: Nullable types and nullable structs
    // TODO: Readonly struct fields
}
