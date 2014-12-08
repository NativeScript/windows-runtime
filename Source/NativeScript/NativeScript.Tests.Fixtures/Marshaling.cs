using System;

namespace NativeScript.Tests.Fixtures
{
    public sealed class MarshalingClass
    {
        private MarshalingClass()
        {
        }

        public void InOutParameterMethod(int inParameter, out int outParameter)
        {
            throw new NotImplementedException();
        }
    }
}
