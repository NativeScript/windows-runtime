#include "pch.h"
#include "Declaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

Declaration::Declaration() {
}

Declaration::~Declaration() {
}

bool Declaration::isExported() const {
    return true;
}

}
}
