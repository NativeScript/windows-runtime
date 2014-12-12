using System.Collections.Generic;
using System.Linq;
using Windows.Data.Json;

namespace CSharpGameraww
{
    public sealed class JsonVectorView : IReadOnlyList<object>
    {
        private readonly JsonArray array;

        internal JsonVectorView(JsonArray array)
        {
            this.array = array;
        }

        public object this[int index]
        {
            get { return JsonProperty.ValueToObject(array[index]); }
        }

        public int Count
        {
            get { return array.Count; }
        }

        public IEnumerator<object> GetEnumerator()
        {
            return array.Select(JsonProperty.ValueToObject).GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}