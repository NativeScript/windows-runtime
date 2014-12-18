#include "pch.h"

namespace NativeScript {
namespace Tests {

TEST_CLASS(StructsTests) {
public:
    TEST_METHOD(SimpleStruct) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleStruct"};
        shared_ptr<StructDeclaration> declaration{dynamic_pointer_cast<StructDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"SimpleStruct");
        Assert::IsTrue(declaration->fullName() == name);

        Assert::IsTrue(declaration->size() == 13);

        StructDeclaration::FieldIterator it{declaration->begin()};
        FieldDeclaration booleanField{*it};
        Assert::IsTrue(booleanField.name() == L"BooleanField");
        Assert::IsTrue(booleanField.fullName() == L"BooleanField");

        ++it;
        FieldDeclaration byteField{*it};
        Assert::IsTrue(byteField.name() == L"ByteField");
        Assert::IsTrue(byteField.fullName() == L"ByteField");

        ++it;
        FieldDeclaration charField{*it};
        Assert::IsTrue(charField.name() == L"CharField");
        Assert::IsTrue(charField.fullName() == L"CharField");

        ++it;
        FieldDeclaration doubleField{*it};
        Assert::IsTrue(doubleField.name() == L"DoubleField");
        Assert::IsTrue(doubleField.fullName() == L"DoubleField");

        ++it;
        FieldDeclaration int16Field{*it};
        Assert::IsTrue(int16Field.name() == L"Int16Field");
        Assert::IsTrue(int16Field.fullName() == L"Int16Field");

        ++it;
        FieldDeclaration int32Field{*it};
        Assert::IsTrue(int32Field.name() == L"Int32Field");
        Assert::IsTrue(int32Field.fullName() == L"Int32Field");

        ++it;
        FieldDeclaration int64Field{*it};
        Assert::IsTrue(int64Field.name() == L"Int64Field");
        Assert::IsTrue(int64Field.fullName() == L"Int64Field");

        ++it;
        FieldDeclaration simpleEnumField{*it};
        Assert::IsTrue(simpleEnumField.name() == L"SimpleEnumField");
        Assert::IsTrue(simpleEnumField.fullName() == L"SimpleEnumField");

        ++it;
        FieldDeclaration singleField{*it};
        Assert::IsTrue(singleField.name() == L"SingleField");
        Assert::IsTrue(singleField.fullName() == L"SingleField");

        ++it;
        FieldDeclaration stringField{*it};
        Assert::IsTrue(stringField.name() == L"StringField");
        Assert::IsTrue(stringField.fullName() == L"StringField");

        ++it;
        FieldDeclaration uInt16Field{*it};
        Assert::IsTrue(uInt16Field.name() == L"UInt16Field");
        Assert::IsTrue(uInt16Field.fullName() == L"UInt16Field");

        ++it;
        FieldDeclaration uInt32Field{*it};
        Assert::IsTrue(uInt32Field.name() == L"UInt32Field");
        Assert::IsTrue(uInt32Field.fullName() == L"UInt32Field");

        ++it;
        FieldDeclaration uInt64Field{*it};
        Assert::IsTrue(uInt64Field.name() == L"UInt64Field");
        Assert::IsTrue(uInt64Field.fullName() == L"UInt64Field");

        ++it;
        Assert::IsTrue(it == declaration->end());
    }
};

}
}
