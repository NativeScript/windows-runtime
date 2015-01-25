using System;
using System.Collections;
using System.Collections.Generic;
using Windows.Foundation;

namespace NativeScript.TestFixtures
{
    public interface IEmptyInterface
    {
    }

    public interface IExplicitInterface1
    {
        void Method();
    }

    public interface IExplicitInterface2
    {
        void Method();
    }

    public sealed class ExplicitInterfaceMethodImplementationClass : IExplicitInterface1, IExplicitInterface2
    {
        private ExplicitInterfaceMethodImplementationClass()
        {
        }

        void IExplicitInterface1.Method()
        {
            throw new NotImplementedException();
        }

        void IExplicitInterface2.Method()
        {
            throw new NotImplementedException();
        }

        public void Method()
        {
            throw new NotImplementedException();
        }
    }

    public sealed class SingleGenericImplementationClass : IEnumerable<Int32>
    {
        private SingleGenericImplementationClass()
        {
        }

        public IEnumerator<int> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }

    public sealed class MultiGenericImplementationClass :
        IEnumerable<Boolean>,
        IEnumerable<Byte>,
        IEnumerable<Char>,
        IEnumerable<Int16>,
        IEnumerable<UInt16>,
        IEnumerable<Int32>,
        IEnumerable<UInt32>,
        IEnumerable<Int64>,
        IEnumerable<UInt64>,
        IEnumerable<Single>,
        IEnumerable<Double>,
        IEnumerable<String>,
        IEnumerable<Guid>,
        IEnumerable<Type>,
        IEnumerable<Object>,
        IEnumerable<SimpleEnum>,
        IEnumerable<SimpleStruct>,
        // IEnumerable<ComplexStruct>,
        IEnumerable<Int32?>,
        IEnumerable<SimpleEnum?>,
        IEnumerable<SimpleStruct?>,
        IEnumerable<SimpleClass>,
        IEnumerable<SimpleDelegate>,
        IEnumerable<TypedEventHandler<object, int>>,
        IEnumerable<IExplicitInterface1>,
        IEnumerable<IEnumerable<String>>,
        IEnumerable<IList<String>>,
        IEnumerable<IReadOnlyList<String>>,
        IEnumerable<IDictionary<String, String>>,
        IEnumerable<IReadOnlyDictionary<String, String>>,
        IEnumerable<IDictionary<String, IEnumerable<String>>>
    {
        public MultiGenericImplementationClass()
        {
        }

        IEnumerator<Boolean> IEnumerable<Boolean>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Byte> IEnumerable<Byte>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Char> IEnumerable<Char>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Int16> IEnumerable<Int16>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<UInt16> IEnumerable<UInt16>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Int32> IEnumerable<Int32>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<UInt32> IEnumerable<UInt32>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Int64> IEnumerable<Int64>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<UInt64> IEnumerable<UInt64>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Single> IEnumerable<Single>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Double> IEnumerable<Double>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<String> IEnumerable<String>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Guid> IEnumerable<Guid>.GetEnumerator()
        {
            throw new NotImplementedException();
        }
        IEnumerator<Type> IEnumerable<Type>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Object> IEnumerable<Object>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleEnum> IEnumerable<SimpleEnum>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleStruct> IEnumerable<SimpleStruct>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        // IEnumerator<ComplexStruct> IEnumerable<ComplexStruct>.GetEnumerator()
        // {
        //     throw new NotImplementedException();
        // }

        IEnumerator<Int32?> IEnumerable<Int32?>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleEnum?> IEnumerable<SimpleEnum?>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleStruct?> IEnumerable<SimpleStruct?>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleClass> IEnumerable<SimpleClass>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<SimpleDelegate> IEnumerable<SimpleDelegate>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<TypedEventHandler<object, int>> IEnumerable<TypedEventHandler<object, int>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IExplicitInterface1> IEnumerable<IExplicitInterface1>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IEnumerable<String>> IEnumerable<IEnumerable<String>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IList<String>> IEnumerable<IList<String>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IReadOnlyList<String>> IEnumerable<IReadOnlyList<String>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IDictionary<String, String>> IEnumerable<IDictionary<String, String>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IReadOnlyDictionary<String, String>> IEnumerable<IReadOnlyDictionary<String, String>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<IDictionary<String, IEnumerable<String>>> IEnumerable<IDictionary<String, IEnumerable<String>>>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }
}
