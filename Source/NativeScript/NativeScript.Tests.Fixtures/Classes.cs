using System;

namespace NativeScript.Tests.Fixtures
{
    public sealed class EmptyClass
    {
        private EmptyClass()
        {
        }
    }

    public sealed class SimpleClass
    {
        public int InstanceReadonlyProperty
        {
            get { throw new NotImplementedException(); }
        }

        public static int StaticReadonlyProperty
        {
            get { throw new NotImplementedException(); }
        }

        private SimpleClass()
        {
        }

        public int InstanceMethod()
        {
            throw new NotImplementedException();
        }

        public static int StaticMethod()
        {
            throw new NotImplementedException();
        }
    }

    public sealed class ActivatableClass
    {
        public ActivatableClass()
        {
            throw new NotImplementedException();
        }

        public ActivatableClass(object arg1)
        {
            throw new NotImplementedException();
        }

        public ActivatableClass(object arg1, object arg2)
        {
            throw new NotImplementedException();
        }
    }
}
