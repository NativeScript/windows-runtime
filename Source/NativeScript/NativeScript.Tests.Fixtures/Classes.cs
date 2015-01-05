using System;
using Windows.Foundation.Metadata;

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
        public int InstanceProperty
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }

        public int InstanceReadonlyProperty
        {
            get { throw new NotImplementedException(); }
        }

        public static int StaticProperty
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
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

        [Version(1)]
        public ActivatableClass(object arg1)
        {
            throw new NotImplementedException();
        }

        [Version(1)]
        public ActivatableClass(object arg1, object arg2)
        {
            throw new NotImplementedException();
        }

        [Version(2)]
        public ActivatableClass(object arg1, object arg2, object arg3)
        {
            throw new NotImplementedException();
        }

        [Version(2)]
        public ActivatableClass(object arg1, object arg2, object arg3, object arg4)
        {
            throw new NotImplementedException();
        }

        public ActivatableClass(object arg1, object arg2, object arg3, object arg4, object arg5)
        {
            throw new NotImplementedException();
        }
    }
    }
}
