#include "pch.h"
#include "NamespaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

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