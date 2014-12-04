#include "pch.h"
#include <iomanip>

namespace NativeScript {
namespace Tests {

TEST_CLASS(EnumsTests) {
public:
    TEST_METHOD(EmptyEnum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EmptyEnum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        Assert::IsTrue(declaration->name() == L"EmptyEnum");
        Assert::IsTrue(declaration->fullName() == name);
        Assert::IsTrue(declaration->size() == 0);

        Assert::IsTrue(declaration->begin() == declaration->end());
    }

    TEST_METHOD(SimpleEnum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleEnum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        EnumDeclaration::MemberIterator it{declaration->begin()};
        EnumMemberDeclaration member1{*it};
        Assert::IsTrue(member1.value() == -1);
        Assert::IsTrue(member1.name() == L"Value1");
        Assert::IsTrue(member1.fullName() == L"NativeScript.Tests.Fixtures.SimpleEnum.Value1");

        ++it;
        EnumMemberDeclaration member2{*it};
        Assert::IsTrue(member2.value() == 0);
        Assert::IsTrue(member2.name() == L"Value2");
        Assert::IsTrue(member2.fullName() == L"NativeScript.Tests.Fixtures.SimpleEnum.Value2");

        ++it;
        EnumMemberDeclaration member3{*it};
        Assert::IsTrue(member3.value() == 1);
        Assert::IsTrue(member3.name() == L"Value3");
        Assert::IsTrue(member3.fullName() == L"NativeScript.Tests.Fixtures.SimpleEnum.Value3");

        ++it;
        Assert::IsTrue(it == declaration->end());
    }

    TEST_METHOD(Int32Enum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.Int32Enum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        EnumDeclaration::MemberIterator it{declaration->begin()};
        EnumMemberDeclaration member1{*it};
        Assert::IsTrue(member1.value() == INT32_MIN);
        Assert::IsTrue(member1.name() == L"MinValue");

        ++it;
        EnumMemberDeclaration member2{*it};
        Assert::IsTrue(member2.value() == INT32_MAX);
        Assert::IsTrue(member2.name() == L"MaxValue");

        ++it;
        Assert::IsTrue(it == declaration->end());
    }

    TEST_METHOD(ManyMembersEnum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.ManyMembersEnum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        size_t size{declaration->size()};
        Assert::IsTrue(size == 512);

        EnumDeclaration::MemberIterator it{declaration->begin()};

        for (size_t i = 0; i < size; ++i) {
            EnumMemberDeclaration member{*it};

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
