#include "Metadata-Prefix.h"
#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    Declaration::Declaration(DeclarationKind kind)
        : _kind{ kind } {
    }

    // TODO: Check deprecated?, obsolete
    bool Declaration::isExported() const {
        return true;
    }

    DeclarationKind Declaration::kind() const {
        return _kind;
    }
}
}
