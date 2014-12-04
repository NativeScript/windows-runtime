#include "pch.h"
#include "ClassDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

// TODO
namespace {

vector<MethodDeclaration> makeMethodDeclarations(ComPtr<IMetaDataImport2> metadata, mdTypeDef token) {
    HCORENUM enumerator{nullptr};
    ULONG count{0};
    array<mdProperty, 1024> tokens;

    ASSERT_SUCCESS(metadata->EnumMethods(&enumerator, token, tokens.data(), tokens.size(), &count));
    ASSERT(count < tokens.size() - 1);
    metadata->CloseEnum(enumerator);

    vector<MethodDeclaration> result;
    for (size_t i = 0; i < count; ++i) {
        MethodDeclaration method{metadata, tokens[i]};

        if (!method.isExported()) {
            continue;
        }

        result.push_back(move(method));
    }

    return result;
}

vector<PropertyDeclaration> makePropertyDeclarations(ComPtr<IMetaDataImport2> metadata, mdTypeDef token) {
    HCORENUM enumerator{nullptr};
    ULONG count{0};
    array<mdMethodDef, 1024> tokens;

    ASSERT_SUCCESS(metadata->EnumProperties(&enumerator, token, tokens.data(), tokens.size(), &count));
    ASSERT(count < tokens.size() - 1);
    metadata->CloseEnum(enumerator);

    vector<PropertyDeclaration> result;
    for (size_t i = 0; i < count; ++i) {
        PropertyDeclaration property{metadata, tokens[i]};

        if (!property.isExported()) {
            continue;
        }

        result.push_back(property);
    }

    return result;
}

}

ClassDeclaration::ClassDeclaration(ComPtr<IMetaDataImport2> metadata, mdTypeDef token)
    : Base(metadata, token)
      , _methods(makeMethodDeclarations(metadata, token))
      , _properties(makePropertyDeclarations(metadata, token)) {

}

wstring ClassDeclaration::baseFullName() const {
    identifier parentName;
    ULONG parentNameLength;

    mdToken parentToken{mdTokenNil};
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

    wstring result{parentName.data(), parentNameLength - 1};
    return result;
}

IteratorRange<ClassDeclaration::MethodIterator> ClassDeclaration::methods() const {
    return IteratorRange<MethodIterator>(_methods.begin(), _methods.end());
}

IteratorRange<ClassDeclaration::PropertyIterator> ClassDeclaration::properties() const {
    return IteratorRange<PropertyIterator>(_properties.begin(), _properties.end());
}

}
}
