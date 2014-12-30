using System;
using System.Collections;
using System.Collections.Generic;

namespace NativeScript.Tests.Fixtures
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

    public sealed class DoubleGenericImplementationClass : IEnumerable<Int16>, IEnumerable<Int32>
    {
        private DoubleGenericImplementationClass()
        {
        }

        IEnumerator<Int32> IEnumerable<Int32>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator<Int16> IEnumerable<Int16>.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }
}
