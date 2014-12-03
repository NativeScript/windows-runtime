using System;

namespace NativeScript.Tests.Fixtures
{
    public sealed class SimpleClass
    {
        private const int InstanceReadonlyPropertyBackingField = 0;
        public int InstanceReadonlyProperty
        {
            get { return InstanceReadonlyPropertyBackingField; }
        }

        private const int StaticReadonlyPropertyBackingField = 0;
        public static int StaticReadonlyProperty
        {
            get { return StaticReadonlyPropertyBackingField; }
        }

        public int InstanceMethod()
        {
            throw new NotImplementedException();
        }

        public static int StaticMethod()
        {
            throw new NotImplementedException();
        }

        private SimpleClass()
        {
        }
    }
}
