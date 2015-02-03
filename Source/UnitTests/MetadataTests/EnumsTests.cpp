#include "UnitTests-Prefix.h"
#include <iomanip>

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(EnumsTests){
        public :
            TEST_METHOD(EmptyEnum){
                const wchar_t * name{ L"NativeScript.TestFixtures.EmptyEnum" };
    const EnumDeclaration* declaration{ static_cast<const EnumDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->name() == L"EmptyEnum");
    Assert::IsTrue(declaration->fullName() == name);
    Assert::IsTrue(declaration->size() == 0);

    Assert::IsTrue(declaration->begin() == declaration->end());
}

TEST_METHOD(SimpleEnum) {
    const wchar_t* name{ L"NativeScript.TestFixtures.SimpleEnum" };
    const EnumDeclaration* declaration{ static_cast<const EnumDeclaration*>(MetadataReader::findByName(name)) };

    EnumDeclaration::MemberIterator it{ declaration->begin() };
    const EnumMemberDeclaration& member1{ *it };
    Assert::IsTrue(member1.value() == -1);
    Assert::IsTrue(member1.name() == L"Value1");
    Assert::IsTrue(member1.fullName() == L"Value1");

    ++it;
    const EnumMemberDeclaration& member2{ *it };
    Assert::IsTrue(member2.value() == 0);
    Assert::IsTrue(member2.name() == L"Value2");
    Assert::IsTrue(member2.fullName() == L"Value2");

    ++it;
    const EnumMemberDeclaration& member3{ *it };
    Assert::IsTrue(member3.value() == 1);
    Assert::IsTrue(member3.name() == L"Value3");
    Assert::IsTrue(member3.fullName() == L"Value3");

    ++it;
    Assert::IsTrue(it == declaration->end());
}

TEST_METHOD(Int32Enum) {
    const wchar_t* name{ L"NativeScript.TestFixtures.Int32Enum" };
    const EnumDeclaration* declaration{ static_cast<const EnumDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->type().elementType() == ElementType::Int32);

    EnumDeclaration::MemberIterator it{ declaration->begin() };
    const EnumMemberDeclaration& member1{ *it };
    Assert::IsTrue(member1.value() == INT32_MIN);
    Assert::IsTrue(member1.name() == L"MinValue");

    ++it;
    const EnumMemberDeclaration& member2{ *it };
    Assert::IsTrue(member2.value() == INT32_MAX);
    Assert::IsTrue(member2.name() == L"MaxValue");

    ++it;
    Assert::IsTrue(it == declaration->end());
}

TEST_METHOD(UInt32Enum) {
    const wchar_t* name{ L"NativeScript.TestFixtures.UInt32Enum" };
    const EnumDeclaration* declaration{ static_cast<const EnumDeclaration*>(MetadataReader::findByName(name)) };

    Assert::IsTrue(declaration->type().elementType() == ElementType::UInt32);

    EnumDeclaration::MemberIterator it{ declaration->begin() };
    const EnumMemberDeclaration& member1{ *it };
    Assert::IsTrue(member1.value() == 0);
    Assert::IsTrue(member1.name() == L"MinValue");

    ++it;
    const EnumMemberDeclaration& member2{ *it };
    Assert::IsTrue(member2.value() == UINT32_MAX);
    Assert::IsTrue(member2.name() == L"MaxValue");

    ++it;
    Assert::IsTrue(it == declaration->end());
}

TEST_METHOD(ManyMembersEnum) {
    const wchar_t* name{ L"NativeScript.TestFixtures.ManyMembersEnum" };
    const EnumDeclaration* declaration{ static_cast<const EnumDeclaration*>(MetadataReader::findByName(name)) };

    size_t size{ declaration->size() };
    Assert::IsTrue(size == 512);

    EnumDeclaration::MemberIterator it{ declaration->begin() };

    for (size_t i = 0; i < size; ++i) {
        const EnumMemberDeclaration& member{ *it };

        wostringstream ss;
        ss << L"V" << setw(3) << setfill(L'0') << i;
        Assert::IsTrue(member.name() == ss.str());

        Assert::IsTrue(member.value() == i);

        ++it;
    }

    Assert::IsTrue(it == declaration->end());
}
};
}
}
