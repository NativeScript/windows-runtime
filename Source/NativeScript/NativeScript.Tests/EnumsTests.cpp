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
        Assert::IsTrue(declaration->size() == 0);

        Assert::IsTrue(declaration->begin() == declaration->end());
    }

    TEST_METHOD(SimpleEnum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.SimpleEnum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        EnumDeclaration::MemberIterator it{declaration->begin()};
        Assert::IsTrue((*it).value() == -1);
        Assert::IsTrue((*it).name() == L"Value1");

        ++it;
        Assert::IsTrue((*it).value() == 0);
        Assert::IsTrue((*it).name() == L"Value2");

        ++it;
        Assert::IsTrue((*it).value() == 1);
        Assert::IsTrue((*it).name() == L"Value3");

        ++it;
        Assert::IsTrue(it == declaration->end());
    }

    TEST_METHOD(Int32Enum) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.Int32Enum"};
        shared_ptr<EnumDeclaration> declaration{dynamic_pointer_cast<EnumDeclaration>(metadataReader.findByName(name))};

        EnumDeclaration::MemberIterator it{declaration->begin()};
        Assert::IsTrue((*it).value() == INT32_MIN);
        Assert::IsTrue((*it).name() == L"MinValue");

        ++it;
        Assert::IsTrue((*it).value() == INT32_MAX);
        Assert::IsTrue((*it).name() == L"MaxValue");

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
            wostringstream ss;
            ss << L"V" << setw(3) << setfill(L'0') << i;
            Assert::IsTrue((*it).name() == ss.str());

            Assert::IsTrue((*it).value() == i);

            ++it;
        }

        Assert::IsTrue(it == declaration->end());
    }
};

}
}