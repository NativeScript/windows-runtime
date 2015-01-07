using System;
using Windows.Foundation.Metadata;

namespace NativeScript.TestFixtures
{
    public sealed class OverloadsClass
    {
        private OverloadsClass()
        {
        }

        [DefaultOverload]
        [Overload("InstanceMethod")]
        public double InstanceMethod(double x)
        {
            throw new NotImplementedException();
        }

        [Overload("InstanceMethodWithInt")]
        public int InstanceMethod(int x)
        {
            throw new NotImplementedException();
        }

        [DefaultOverload]
        [Overload("StaticMethod")]
        public static double StaticMethod(double x)
        {
            throw new NotImplementedException();
        }

        [Overload("StaticMethodWithInt")]
        public static int StaticMethod(int x)
        {
            throw new NotImplementedException();
        }
    }
}
