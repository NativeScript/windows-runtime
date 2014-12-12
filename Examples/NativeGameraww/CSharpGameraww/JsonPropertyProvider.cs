using System;
using Windows.Data.Json;
using Windows.UI.Xaml.Data;

namespace CSharpGameraww
{
    public sealed class JsonPropertyProvider : ICustomPropertyProvider
    {
        private readonly JsonObject @object;

        internal JsonObject Object
        {
            get { return @object; }
        }

        internal JsonPropertyProvider(JsonObject @object)
        {
            this.@object = @object;
        }

        public ICustomProperty GetCustomProperty(string name)
        {
            IJsonValue value;
            if (@object.TryGetValue(name, out value))
            {
                return new JsonProperty(name);
            }

            return null;
        }

        public ICustomProperty GetIndexedProperty(string name, Type type)
        {
            return null;
        }

        public string GetStringRepresentation()
        {
            return @object.ToString();
        }

        public Type Type
        {
            get { return GetType(); }
        }
    }
}
