using System;
using Windows.Foundation;

namespace NativeScript.Tests.Fixtures
{
#pragma warning disable 67
    public sealed class EventsClass
    {
        private EventsClass()
        {
            throw new NotImplementedException();
        }

        public static event EmptyDelegate StaticEvent;
        public event EmptyDelegate EmptyEvent;
        public event Windows.UI.Xaml.RoutedEventHandler ExternalEvent;
        public event TypedEventHandler<object, int> GenericEvent;
    }
#pragma warning restore 67
}
