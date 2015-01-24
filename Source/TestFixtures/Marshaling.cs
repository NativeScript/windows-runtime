using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;

namespace NativeScript.TestFixtures
{
    public sealed class MarshalingClass
    {
        private MarshalingClass()
        {
        }

        public void InOutParameterMethod(int inParameter, out long outParameter)
        {
            throw new NotImplementedException();
        }

        public Boolean MethodWithBoolean(Boolean x)
        {
            return x;
        }

        public Byte MethodWithByte(Byte x)
        {
            return x;
        }

        public Char MethodWithChar(Char x)
        {
            return x;
        }

        public Int16 MethodWithInt16(Int16 x)
        {
            return x;
        }

        public UInt16 MethodWithUInt16(UInt16 x)
        {
            return x;
        }

        public Int32 MethodWithInt32(Int32 x)
        {
            return x;
        }

        public UInt32 MethodWithUInt32(UInt32 x)
        {
            return x;
        }

        public Int64 MethodWithInt64(Int64 x)
        {
            return x;
        }

        public UInt64 MethodWithUInt64(UInt64 x)
        {
            return x;
        }

        public Single MethodWithSingle(Single x)
        {
            return x;
        }

        public Double MethodWithDouble(Double x)
        {
            return x;
        }

        public String MethodWithString(String x)
        {
            return x;
        }

        public Guid MethodWithGuid(Guid x)
        {
            return x;
        }

        public Type MethodWithType(Type x)
        {
            return x;
        }

        public Object MethodWithObject(Object x)
        {
            return x;
        }

        public SimpleEnum MethodWithEnum(SimpleEnum x)
        {
            return x;
        }

        public SimpleStruct MethodWithStruct(SimpleStruct x)
        {
            return x;
        }

        public DateTimeOffset MethodWithDateTime(DateTimeOffset x)
        {
            return x;
        }

        public TimeSpan MethodWithTimeSpan(TimeSpan x)
        {
            return x;
        }

        public String[] MethodWithReadOnlyArray([ReadOnlyArray] String[] x)
        {
            return x;
        }

        public String[] MethodWithWriteOnlyArray([WriteOnlyArray] String[] x)
        {
            return x;
        }

        public SimpleClass[] MethodWithClassArray([ReadOnlyArray] SimpleClass[] x)
        {
            return x;
        }

        public Int32? MethodWithNullableInt32(Int32? x)
        {
            return x;
        }

        public SimpleEnum? MethodWithNullableEnum(SimpleEnum? x)
        {
            return x;
        }

        public SimpleStruct? MethodWithNullableStruct(SimpleStruct? x)
        {
            return x;
        }

        public SimpleClass MethodWithClass(SimpleClass x)
        {
            return x;
        }

        public SimpleDelegate MethodWithSimpleDelegate(SimpleDelegate x)
        {
            return x;
        }

        public TypedEventHandler<object, int> MethodWithGenericDelegate(TypedEventHandler<object, int> x)
        {
            return x;
        }

        public IExplicitInterface1 MethodWithInterface(IExplicitInterface1 x)
        {
            return x;
        }

        public IEnumerable<String> MethodWithIIterable(IEnumerable<String> x)
        {
            return x;
        }

        // public IEnumerator<String> MethodWithIIterator(IEnumerator<String> x)
        // {
        //     return x;
        // }

        public IList<String> MethodWithIVector(IList<String> x)
        {
            return x;
        }

        public IReadOnlyList<String> MethodWithIVectorView(IReadOnlyList<String> x)
        {
            return x;
        }

        public IDictionary<String, String> MethodWithIMap(IDictionary<String, String> x)
        {
            return x;
        }

        public IReadOnlyDictionary<String, String> MethodWithIMapView(IReadOnlyDictionary<String, String> x)
        {
            return x;
        }

        public IDictionary<String, IEnumerable<String>> MethodWithMultiMap(IDictionary<String, IEnumerable<String>> x)
        {
            return x;
        }
    }
}
