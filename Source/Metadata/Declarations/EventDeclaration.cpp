#include "Metadata-Prefix.h"
#include "EventDeclaration.h"
#include "DeclarationFactory.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    namespace {

        MethodDeclaration makeAddMethod(IMetaDataImport2* metadata, mdEvent token) {
            mdMethodDef addMethodToken{ mdTokenNil };
            ASSERT_SUCCESS(metadata->GetEventProps(token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, &addMethodToken, nullptr, nullptr, nullptr, 0, nullptr));

            return MethodDeclaration{ metadata, addMethodToken };
        }

        MethodDeclaration makeRemoveMethod(IMetaDataImport2* metadata, mdEvent token) {
            mdMethodDef removeMethodToken{ mdTokenNil };
            ASSERT_SUCCESS(metadata->GetEventProps(token, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, &removeMethodToken, nullptr, nullptr, 0, nullptr));

            return MethodDeclaration{ metadata, removeMethodToken };
        }

        unique_ptr<DelegateDeclaration> makeType(IMetaDataImport2* metadata, mdEvent token) {
            mdToken delegateToken{ mdTokenNil };
            ASSERT_SUCCESS(metadata->GetEventProps(token, nullptr, nullptr, 0, nullptr, nullptr, &delegateToken, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

            return DeclarationFactory::makeDelegateDeclaration(metadata, delegateToken);
        }
    }

    EventDeclaration::EventDeclaration(IMetaDataImport2* metadata, mdEvent token)
        : Base(DeclarationKind::Event)
        , _metadata{ metadata }
        , _token{ token }
        , _type{ makeType(metadata, token) }
        , _addMethod{ makeAddMethod(metadata, token) }
        , _removeMethod{ makeRemoveMethod(metadata, token) } {

        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtEvent);
        ASSERT(token != mdEventNil);
    }

    bool EventDeclaration::isExported() const {
        DWORD flags{ 0 };
        ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

        if (IsEvSpecialName(flags)) {
            return false;
        }

        return true;
    }

    wstring EventDeclaration::name() const {
        return fullName();
    }

    wstring EventDeclaration::fullName() const {
        identifier nameData;
        ULONG nameDataLength{ 0 };

        ASSERT_SUCCESS(_metadata->GetEventProps(_token, nullptr, nameData.data(), nameData.size(), &nameDataLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr));

        wstring name{ nameData.data(), nameDataLength - 1 };
        return name;
    }

    bool EventDeclaration::isStatic() const {
        return addMethod().isStatic();
    }

    bool EventDeclaration::isSealed() const {
        return addMethod().isSealed();
    }

    const DelegateDeclaration& EventDeclaration::type() const {
        return *_type.get();
    }

    const MethodDeclaration& EventDeclaration::addMethod() const {
        return _addMethod;
    }

    const MethodDeclaration& EventDeclaration::removeMethod() const {
        return _removeMethod;
    }
}
}
