#include "pch.h"
#include "Metadata.h"

namespace NativeScript {
namespace Tests {

using namespace Metadata;
using namespace Fixtures;

TEST_CLASS(MetadataTest) {
public:
    TEST_METHOD(TestMethod1) {
        MetadataReader metadataReader = MetadataReader::instance();
        Class1 class1;
    }
};

}
}