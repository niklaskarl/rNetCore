#include "../gtest/include/gtest/gtest.h"
#include "../../package/src/stringtranscoding/stringtranscoding.h"

namespace {

class Utf16ToUtf8IteratorTest : public ::testing::Test {
protected:
    Utf16ToUtf8IteratorTest()
    { }

    ~Utf16ToUtf8IteratorTest() override
    { }

    void SetUp() override
    { }

    void TearDown() override
    { }
};

TEST_F(Utf16ToUtf8IteratorTest, IterateString) {
    std::u16string source = u"€,®T𐐷st𤭢";
    std::string expected = u8"€,®T𐐷st𤭢";

    CREATE_ITER_U16_U8(begin, const char16_t *, source.c_str());
    CREATE_ITER_U16_U8(iterator, const char16_t *, source.c_str());
    for (size_t i = 0; i < expected.length(); ++i)
    {
        *iterator;
        *iterator;
        EXPECT_EQ(*iterator, expected[i]);

        ++iterator;
    }

    ASSERT_EQ(*iterator, L'\0');

    for (size_t i = 0; i < expected.length(); ++i)
    {
        --iterator;

        *iterator;
        *iterator;
        EXPECT_EQ(*iterator, expected[expected.length() - i - 1]);
    }

    ASSERT_EQ(iterator, begin);
}

} // namespace
