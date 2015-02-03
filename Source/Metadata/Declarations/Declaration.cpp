#include "Metadata-Prefix.h"
#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    // TODO: Check deprecated?, obsolete
    bool Declaration::isExported() const {
        return true;
    }
}
}
