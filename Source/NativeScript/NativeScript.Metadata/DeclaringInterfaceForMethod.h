#pragma once

#include <memory>

#include "InterfaceDeclaration.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

std::unique_ptr<const InterfaceDeclaration> findDeclaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

}
}
