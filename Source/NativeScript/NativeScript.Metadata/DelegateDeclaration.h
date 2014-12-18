#pragma once

#include <guiddef.h>

#include "TypeDeclaration.h"
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

class DelegateDeclaration final : public TypeDeclaration {
public:
    typedef TypeDeclaration Base;

    using ParameterIterator = MethodDeclaration::ParameterIterator;

    explicit DelegateDeclaration(IMetaDataImport2*, mdTypeDef);

    CLSID id() const;

    IteratorRange<ParameterIterator> parameters() const;

    size_t numberOfParameters() const;

private:
    MethodDeclaration _invokeMethod;
};

}
}
