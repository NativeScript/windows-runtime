using System;
using Windows.Foundation.Metadata;

namespace NativeScript.Tests.Fixtures
{
    public sealed class OverloadsClass
    {
        private OverloadsClass()
        {
        }

        [DefaultOverload]
        [Overload("InstanceMethodWithDouble")]
        public double InstanceMethod(double x)
        {
            throw new NotImplementedException();
        }

        [Overload("InstanceMethodWithInt")]
        public int InstanceMethod(int x)
        {
            throw new NotImplementedException();
        }

        public double InstanceMethodWithDouble(double x)
        {
            return InstanceMethod(x);
        }

        public int InstanceMethodWithInt(int x)
        {
            return InstanceMethod(x);
        }

        [DefaultOverload]
        [Overload("StaticMethodWithDouble")]
        public static double StaticMethod(double x)
        {
            throw new NotImplementedException();
        }

        [Overload("StaticMethodWithInt")]
        public static int StaticMethod(int x)
        {
            throw new NotImplementedException();
        }

        public static double StaticMethodWithDouble(double x)
        {
            return StaticMethod(x);
        }

        public static int StaticMethodWithInt(int x)
        {
            return StaticMethod(x);
        }
    }
}
