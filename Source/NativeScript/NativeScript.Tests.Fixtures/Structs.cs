using System;

namespace NativeScript.Tests.Fixtures
{
    // Keep fields sorted by field name

    public struct SimpleStruct
    {
        public Boolean BooleanField;
        public Byte ByteField;
        public Char CharField;
        public Double DoubleField;
        public Int16 Int16Field;
        public Int32 Int32Field;
        public Int64 Int64Field;
        public Single SingleField;
        public String StringField;
        public UInt16 UInt16Field;
        public UInt32 UInt32Field;
        public UInt64 UInt64Field;
    }

    public struct ComplexStruct
    {
        public SimpleEnum? NullableEnumField;
        public Int32? NullableInt32Field;
        public SimpleStruct? NullableStructField;
        public SimpleEnum EnumField;
        public SimpleStruct StructField;
    }
}
