#include "pch.h"
#include "Metadata.h"
#include <mutex>

namespace NativeScript {
namespace Metadata {

using namespace std;

MetadataReader::MetadataReader() {
}

MetadataReader& MetadataReader::instance() {
    static once_flag once;
    static MetadataReader* metadataReader;
    call_once(once, [] {
        metadataReader = new MetadataReader();
    });
    return *metadataReader;
}
}
}