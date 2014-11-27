#pragma once

#include <string>

namespace NativeScript {
namespace Metadata {
namespace Ast {

class Declaration {
public:
    virtual ~Declaration();

    virtual std::wstring name() const = 0;

protected:
    explicit Declaration();
};

}
}
}