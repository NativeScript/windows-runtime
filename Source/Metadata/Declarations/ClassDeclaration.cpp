#include "Metadata-Prefix.h"

#include "ClassDeclaration.h"
#include "InterfaceDeclaration.h"
#include "DeclarationFactory.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    // TODO
    namespace {

        const wchar_t* const DEFAULT_ATTRIBUTE{ L"Windows.Foundation.Metadata.DefaultAttribute" };

        vector<MethodDeclaration> makeInitializerDeclarations(IMetaDataImport2* metadata, mdTypeDef token) {
            HCORENUM enumerator{ nullptr };
            ULONG count{ 0 };
            array<mdProperty, 1024> tokens;

            ASSERT_SUCCESS(metadata->EnumMethodsWithName(&enumerator, token, COR_CTOR_METHOD_NAME_W, tokens.data(), tokens.size(), &count));
            ASSERT(count < tokens.size() - 1);
            metadata->CloseEnum(enumerator);

            vector<MethodDeclaration> result;
            for (size_t i = 0; i < count; ++i) {
                mdMethodDef methodToken{ tokens[i] };

                // TODO: Make a InstanceInitializerDeclaration and check this in it's isExported method
                DWORD flags{ 0 };
                ASSERT_SUCCESS(metadata->GetMethodProps(methodToken, nullptr, nullptr, 0, nullptr, &flags, nullptr, nullptr, nullptr, nullptr));

                if (!IsMdPublic(flags)) {
                    continue;
                }

                result.emplace_back(metadata, methodToken);
            }

            return result;
        }

        unique_ptr<InterfaceDeclaration> makeDefaultInterface(IMetaDataImport2* metadata, mdTypeDef token) {
            array<mdInterfaceImpl, 1024> interfaceImplTokens;
            ULONG interfaceImplCount{ 0 };
            HCORENUM interfaceEnumerator{ nullptr };
            ASSERT_SUCCESS(metadata->EnumInterfaceImpls(&interfaceEnumerator, token, interfaceImplTokens.data(), interfaceImplTokens.size(), &interfaceImplCount));
            ASSERT(interfaceImplCount < interfaceImplTokens.size());
            metadata->CloseEnum(interfaceEnumerator);

            for (size_t i = 0; i < interfaceImplCount; ++i) {
                mdInterfaceImpl interfaceImplToken{ interfaceImplTokens[i] };
                HRESULT getCustomAttributeResult{ metadata->GetCustomAttributeByName(interfaceImplToken, DEFAULT_ATTRIBUTE, nullptr, nullptr) };
                ASSERT_SUCCESS(getCustomAttributeResult);

                if (getCustomAttributeResult == S_OK) {
                    mdToken interfaceToken{ mdTokenNil };
                    ASSERT_SUCCESS(metadata->GetInterfaceImplProps(interfaceImplToken, nullptr, &interfaceToken));
                    return DeclarationFactory::makeInterfaceDeclaration(metadata, interfaceToken);
                }
            }

            ASSERT_NOT_REACHED();
        }
    }

    ClassDeclaration::ClassDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(DeclarationKind::Class, metadata, token)
        , _initializers(makeInitializerDeclarations(metadata, token))
        , _defaultInterface(makeDefaultInterface(metadata, token)) {
    }

    wstring ClassDeclaration::baseFullName() const {
        identifier parentName;
        ULONG parentNameLength;

        mdToken parentToken{ mdTokenNil };
        ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, nullptr, 0, nullptr, nullptr, &parentToken));

        switch (TypeFromToken(parentToken)) {
        case mdtTypeDef: {
            ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentName.data(), parentName.size(), &parentNameLength, nullptr, nullptr));
            break;
        }

        case mdtTypeRef: {
            ASSERT_SUCCESS(_metadata->GetTypeRefProps(parentToken, nullptr, parentName.data(), parentName.size(), &parentNameLength));
            break;
        }

        default:
            ASSERT_NOT_REACHED();
        }

        wstring result{ parentName.data(), parentNameLength - 1 };
        return result;
    }

    const InterfaceDeclaration& ClassDeclaration::defaultInterface() const {
        return *_defaultInterface.get();
    }

    bool ClassDeclaration::isInstantiable() const {
        IteratorRange<MethodIterator> constructors{ initializers() };
        return constructors.begin() != constructors.end();
    }

    bool ClassDeclaration::isSealed() const {
        DWORD flags{ 0 };
        ASSERT_SUCCESS(_metadata->GetTypeDefProps(_token, nullptr, 0, nullptr, &flags, nullptr));
        return IsTdSealed(flags) != 0;
    }

    IteratorRange<ClassDeclaration::MethodIterator> ClassDeclaration::initializers() const {
        return IteratorRange<MethodIterator>(_initializers.begin(), _initializers.end());
    }
}
}
