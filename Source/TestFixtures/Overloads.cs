using System;
using Windows.Foundation.Metadata;

namespace NativeScript.TestFixtures
{
    public sealed class OverloadsClass
    {
        public OverloadsClass()
        {
        }

        public OverloadsClass(double x)
        {
        }

        public OverloadsClass(double x, double y)
        {
        }

        [DefaultOverload]
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
