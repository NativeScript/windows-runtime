#include "UnitTests-Prefix.h"

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(StructsTests){
        public :
            TEST_METHOD(SimpleStruct){
                const wchar_t * name{ L"NativeScript.TestFixtures.SimpleStruct" };
    const StructDeclaration* declaration{ static_cast<const StructDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->name() == L"SimpleStruct");
    Assert::IsTrue(declaration->fullName() == name);

    Assert::IsTrue(declaration->size() == 12);

    StructDeclaration::FieldIterator it{ declaration->begin() };
    const StructFieldDeclaration& booleanField{ *it };
    Assert::IsTrue(booleanField.name() == L"BooleanField");
    Assert::IsTrue(booleanField.fullName() == L"BooleanField");
    Assert::IsTrue(booleanField.type().elementType() == ElementType::Boolean);

    ++it;
    const StructFieldDeclaration& byteField{ *it };
    Assert::IsTrue(byteField.name() == L"ByteField");
    Assert::IsTrue(byteField.fullName() == L"ByteField");
    Assert::IsTrue(byteField.type().elementType() == ElementType::UInt8);

    ++it;
    const StructFieldDeclaration& charField{ *it };
    Assert::IsTrue(charField.name() == L"CharField");
    Assert::IsTrue(charField.fullName() == L"CharField");
    Assert::IsTrue(charField.type().elementType() == ElementType::Char16);

    ++it;
    const StructFieldDeclaration& doubleField{ *it };
    Assert::IsTrue(doubleField.name() == L"DoubleField");
    Assert::IsTrue(doubleField.fullName() == L"DoubleField");
    Assert::IsTrue(doubleField.type().elementType() == ElementType::Double);

    ++it;
    const StructFieldDeclaration& int16Field{ *it };
    Assert::IsTrue(int16Field.name() == L"Int16Field");
    Assert::IsTrue(int16Field.fullName() == L"Int16Field");
    Assert::IsTrue(int16Field.type().elementType() == ElementType::Int16);

    ++it;
    const StructFieldDeclaration& int32Field{ *it };
    Assert::IsTrue(int32Field.name() == L"Int32Field");
    Assert::IsTrue(int32Field.fullName() == L"Int32Field");
    Assert::IsTrue(int32Field.type().elementType() == ElementType::Int32);

    ++it;
    const StructFieldDeclaration& int64Field{ *it };
    Assert::IsTrue(int64Field.name() == L"Int64Field");
    Assert::IsTrue(int64Field.fullName() == L"Int64Field");
    Assert::IsTrue(int64Field.type().elementType() == ElementType::Int64);

    ++it;
    const StructFieldDeclaration& singleField{ *it };
    Assert::IsTrue(singleField.name() == L"SingleField");
    Assert::IsTrue(singleField.fullName() == L"SingleField");
    Assert::IsTrue(singleField.type().elementType() == ElementType::Single);

    ++it;
    const StructFieldDeclaration& stringField{ *it };
    Assert::IsTrue(stringField.name() == L"StringField");
    Assert::IsTrue(stringField.fullName() == L"StringField");
    Assert::IsTrue(stringField.type().elementType() == ElementType::String);

    ++it;
    const StructFieldDeclaration& uInt16Field{ *it };
    Assert::IsTrue(uInt16Field.name() == L"UInt16Field");
    Assert::IsTrue(uInt16Field.fullName() == L"UInt16Field");
    Assert::IsTrue(uInt16Field.type().elementType() == ElementType::UInt16);

    ++it;
    const StructFieldDeclaration& uInt32Field{ *it };
    Assert::IsTrue(uInt32Field.name() == L"UInt32Field");
    Assert::IsTrue(uInt32Field.fullName() == L"UInt32Field");
    Assert::IsTrue(uInt32Field.type().elementType() == ElementType::UInt32);

    ++it;
    const StructFieldDeclaration& uInt64Field{ *it };
    Assert::IsTrue(uInt64Field.name() == L"UInt64Field");
    Assert::IsTrue(uInt64Field.fullName() == L"UInt64Field");
    Assert::IsTrue(uInt64Field.type().elementType() == ElementType::UInt64);

    ++it;
    Assert::IsTrue(it == declaration->end());
}
};
}
}
