using System;
using Windows.Data.Json;
using Windows.UI.Xaml.Data;

namespace CSharpGameraww
{
    public sealed class JsonProperty : ICustomProperty
    {
        private readonly string name;

        internal JsonProperty(string name)
        {
            this.name = name;
        }

        public bool CanRead
        {
            get { return true; }
        }

        public bool CanWrite
        {
            get { return true; }
        }

        public object GetIndexedValue(object target, object index)
        {
            return null;
        }

        public object GetValue(object target)
        {
            var provider = target as JsonPropertyProvider;
            if (provider != null)
            {
                var value = provider.Object[name];
                return ValueToObject(value);
            }
            return null;
        }

        public string Name
        {
            get { return name; }
        }

        public void SetIndexedValue(object target, object value, object index)
        {
        }

        public void SetValue(object target, object value)
        {
        }

        public Type Type
        {
            get { return GetType(); }
        }

        internal static object ValueToObject(IJsonValue value)
        {
            switch (value.ValueType)
            {
                case JsonValueType.Null:
                    return null;
                case JsonValueType.Boolean:
                    return value.GetBoolean();
                case JsonValueType.Number:
                    return value.GetNumber();
                case JsonValueType.String:
                    return value.GetString();
                case JsonValueType.Object:
                    return new JsonPropertyProvider(value.GetObject());
                case JsonValueType.Array:
                    return new JsonVectorView(value.GetArray());
                default:
                    throw new ArgumentException();
            }
        }
    }
}
