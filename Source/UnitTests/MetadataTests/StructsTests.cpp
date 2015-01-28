#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(StructsTests){
        public :
            TEST_METHOD(SimpleStruct){
                MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleStruct" };
    shared_ptr<StructDeclaration> declaration{ static_pointer_cast<StructDeclaration>(metadataReader.findByName(name)) };

    Assert::IsTrue(declaration->name() == L"SimpleStruct");
    Assert::IsTrue(declaration->fullName() == name);

    Assert::IsTrue(declaration->size() == 12);

    StructDeclaration::FieldIterator it{ declaration->begin() };
    const StructFieldDeclaration& booleanField{ *it };
    Assert::IsTrue(booleanField.name() == L"BooleanField");
    Assert::IsTrue(booleanField.fullName() == L"BooleanField");
    PCCOR_SIGNATURE booleanFieldType{ booleanField.type() };
    Assert::IsTrue(CorSigUncompressElementType(booleanFieldType) == ELEMENT_TYPE_BOOLEAN);

    ++it;
    const StructFieldDeclaration& byteField{ *it };
    Assert::IsTrue(byteField.name() == L"ByteField");
    Assert::IsTrue(byteField.fullName() == L"ByteField");
    PCCOR_SIGNATURE byteFieldType{ byteField.type() };
    Assert::IsTrue(CorSigUncompressElementType(byteFieldType) == ELEMENT_TYPE_U1);

    ++it;
    const StructFieldDeclaration& charField{ *it };
    Assert::IsTrue(charField.name() == L"CharField");
    Assert::IsTrue(charField.fullName() == L"CharField");
    PCCOR_SIGNATURE charFieldType{ charField.type() };
    Assert::IsTrue(CorSigUncompressElementType(charFieldType) == ELEMENT_TYPE_CHAR);

    ++it;
    const StructFieldDeclaration& doubleField{ *it };
    Assert::IsTrue(doubleField.name() == L"DoubleField");
    Assert::IsTrue(doubleField.fullName() == L"DoubleField");
    PCCOR_SIGNATURE doubleFieldType{ doubleField.type() };
    Assert::IsTrue(CorSigUncompressElementType(doubleFieldType) == ELEMENT_TYPE_R8);

    ++it;
    const StructFieldDeclaration& int16Field{ *it };
    Assert::IsTrue(int16Field.name() == L"Int16Field");
    Assert::IsTrue(int16Field.fullName() == L"Int16Field");
    PCCOR_SIGNATURE int16FieldType{ int16Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(int16FieldType) == ELEMENT_TYPE_I2);

    ++it;
    const StructFieldDeclaration& int32Field{ *it };
    Assert::IsTrue(int32Field.name() == L"Int32Field");
    Assert::IsTrue(int32Field.fullName() == L"Int32Field");
    PCCOR_SIGNATURE int32FieldType{ int32Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(int32FieldType) == ELEMENT_TYPE_I4);

    ++it;
    const StructFieldDeclaration& int64Field{ *it };
    Assert::IsTrue(int64Field.name() == L"Int64Field");
    Assert::IsTrue(int64Field.fullName() == L"Int64Field");
    PCCOR_SIGNATURE int64FieldType{ int64Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(int64FieldType) == ELEMENT_TYPE_I8);

    ++it;
    const StructFieldDeclaration& singleField{ *it };
    Assert::IsTrue(singleField.name() == L"SingleField");
    Assert::IsTrue(singleField.fullName() == L"SingleField");
    PCCOR_SIGNATURE singleFieldType{ singleField.type() };
    Assert::IsTrue(CorSigUncompressElementType(singleFieldType) == ELEMENT_TYPE_R4);

    ++it;
    const StructFieldDeclaration& stringField{ *it };
    Assert::IsTrue(stringField.name() == L"StringField");
    Assert::IsTrue(stringField.fullName() == L"StringField");
    PCCOR_SIGNATURE stringFieldType{ stringField.type() };
    Assert::IsTrue(CorSigUncompressElementType(stringFieldType) == ELEMENT_TYPE_STRING);

    ++it;
    const StructFieldDeclaration& uInt16Field{ *it };
    Assert::IsTrue(uInt16Field.name() == L"UInt16Field");
    Assert::IsTrue(uInt16Field.fullName() == L"UInt16Field");
    PCCOR_SIGNATURE uInt16FieldType{ uInt16Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(uInt16FieldType) == ELEMENT_TYPE_U2);

    ++it;
    const StructFieldDeclaration& uInt32Field{ *it };
    Assert::IsTrue(uInt32Field.name() == L"UInt32Field");
    Assert::IsTrue(uInt32Field.fullName() == L"UInt32Field");
    PCCOR_SIGNATURE uInt32FieldType{ uInt32Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(uInt32FieldType) == ELEMENT_TYPE_U4);

    ++it;
    const StructFieldDeclaration& uInt64Field{ *it };
    Assert::IsTrue(uInt64Field.name() == L"UInt64Field");
    Assert::IsTrue(uInt64Field.fullName() == L"UInt64Field");
    PCCOR_SIGNATURE uInt64FieldType{ uInt64Field.type() };
    Assert::IsTrue(CorSigUncompressElementType(uInt64FieldType) == ELEMENT_TYPE_U8);

    ++it;
    Assert::IsTrue(it == declaration->end());
}
};
}
}
