using System;
using System.Collections.Generic;
using Windows.Foundation;
using Windows.UI.Xaml;

namespace NativeScript.TestFixtures
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
        public event RoutedEventHandler ExternalEvent;
        public event TypedEventHandler<object, IEnumerable<int>> GenericEvent;
    }
#pragma warning restore 67
}
