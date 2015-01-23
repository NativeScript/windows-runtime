#include "Metadata-Prefix.h"
#include "PropertyDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    namespace {

        MethodDeclaration makeGetter(IMetaDataImport2* metadata, mdProperty token) {
            mdMethodDef getterToken{ mdTokenNil };

            ASSERT_SUCCESS(metadata->GetPropertyProps(token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &getterToken, nullptr, 0, nullptr));
            ASSERT(getterToken != mdMethodDefNil);

            return MethodDeclaration{ metadata, getterToken };
        }

        unique_ptr<MethodDeclaration> makeSetter(IMetaDataImport2* metadata, mdProperty token) {
            mdMethodDef setterToken{ mdTokenNil };

            ASSERT_SUCCESS(metadata->GetPropertyProps(token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &setterToken, nullptr, nullptr, 0, nullptr));

            if (setterToken == mdMethodDefNil) {
                return nullptr;
            }

            return make_unique<MethodDeclaration>(metadata, setterToken);
        }
    }

    PropertyDeclaration::PropertyDeclaration(IMetaDataImport2* metadata, mdProperty token)
        : Base(DeclarationKind::Property)
        , _metadata{ metadata }
        , _token{ token }
        , _getter{ makeGetter(metadata, token) }
        , _setter{ makeSetter(metadata, token) } {

        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtProperty);
        ASSERT(token != mdPropertyNil);
    }

    bool PropertyDeclaration::isExported() const {
        DWORD propertyFlags{ 0 };
        ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, &propertyFlags, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

        if (IsPrSpecialName(propertyFlags)) {
            return false;
        }

        return true;
    }

    wstring PropertyDeclaration::name() const {
        return fullName();
    }

    wstring PropertyDeclaration::fullName() const {
        identifier nameData;
        ULONG nameLength{ 0 };

        ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

        wstring result{ nameData.data(), nameLength - 1 };
        return result;
    }

    bool PropertyDeclaration::isStatic() const {
        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureCount{ 0 };

        ASSERT_SUCCESS(_metadata->GetPropertyProps(_token, nullptr, nullptr, 0, nullptr, nullptr, &signature, &signatureCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

        ASSERT(signatureCount > 0);

        return (signature[0] & IMAGE_CEE_CS_CALLCONV_HASTHIS) == 0;
    }

    bool PropertyDeclaration::isSealed() const {
        return _getter.isSealed();
    }

    const MethodDeclaration& PropertyDeclaration::getter() const {
        return _getter;
    }

    const MethodDeclaration* PropertyDeclaration::setter() const {
        return _setter.get();
    }
}
}
