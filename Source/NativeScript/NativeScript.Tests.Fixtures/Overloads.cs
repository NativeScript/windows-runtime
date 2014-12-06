using System;
using Windows.Foundation.Metadata;

namespace NativeScript.Tests.Fixtures
{
    public sealed class MethodOverloads
    {
        private MethodOverloads()
        {
        }

        [DefaultOverload]
        [Overload("MethodWithDouble")]
        public double Method(double x)
        {
            throw new NotImplementedException();
        }

        [Overload("MethodWithInt")]
        public int Method(int x)
        {
            throw new NotImplementedException();
        }

        public double MethodWithDouble(double x)
        {
            return Method(x);
        }

        public int MethodWithInt(int x)
        {
            return Method(x);
        }
    }
}
