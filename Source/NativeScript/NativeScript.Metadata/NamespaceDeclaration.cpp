#include "pch.h"
#include "NamespaceDeclaration.h"

namespace NativeScript {
namespace Metadata {
namespace Ast {

using namespace std;

NamespaceDeclaration::NamespaceDeclaration(const wchar_t* name)
    : Base()
      , _name(name) {
}

wstring NamespaceDeclaration::name() const {
    return _name;
}

}
}
}