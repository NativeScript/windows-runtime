#include "pch.h"
#include "InterfaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;


InterfaceDeclaration::InterfaceDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
    : Base(metadata, token) {

}

IID InterfaceDeclaration::id() const {
    return getGuidAttributeValue(_metadata.Get(), _token);
}

}
}
