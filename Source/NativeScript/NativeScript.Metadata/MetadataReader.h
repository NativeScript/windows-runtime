#pragma once

#include <memory>

#include "ast.h"

namespace NativeScript {
namespace Metadata {

class MetadataReader {
public:
    std::shared_ptr<Ast::Declaration> findByName(const wchar_t* fullyQualifiedName) const;

    std::shared_ptr<Ast::Declaration> findByNameInNamespace(const wchar_t* name, const wchar_t* namespaceName) const;
};

}
}