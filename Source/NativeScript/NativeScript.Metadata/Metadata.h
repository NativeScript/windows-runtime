#pragma once

namespace NativeScript {
namespace Metadata {

class MetadataReader {
public:
    static MetadataReader& instance();

private:
    MetadataReader();
};
}

}