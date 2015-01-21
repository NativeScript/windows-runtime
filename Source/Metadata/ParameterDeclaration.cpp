#include "Metadata-Prefix.h"
#include "ParameterDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    ParameterDeclaration::ParameterDeclaration(IMetaDataImport2* metadata, mdParamDef token)
        : Base(DeclarationKind::Parameter)
        , _metadata{ metadata }
        , _token{ token } {

        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtParamDef);
        ASSERT(token != mdParamDefNil);
    }

    wstring ParameterDeclaration::name() const {
        return fullName();
    }

    wstring ParameterDeclaration::fullName() const {
        identifier nameData;
        ULONG nameLength{ 0 };

        ASSERT_SUCCESS(_metadata->GetParamProps(_token, nullptr, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr));

        wstring result{ nameData.data(), nameLength - 1 };
        return result;
    }

    bool ParameterDeclaration::isOut() const {
        DWORD flags{ 0 };
        ASSERT_SUCCESS(_metadata->GetParamProps(_token, nullptr, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr));

        return IsPdOut(flags) != 0;
    }
}
}
