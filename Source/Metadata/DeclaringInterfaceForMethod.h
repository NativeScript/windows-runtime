#pragma once

#include <memory>

#include "Declarations/MethodDeclaration.h"
#include "Declarations/InterfaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

    std::unique_ptr<const InterfaceDeclaration> findDeclaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);
}
}
