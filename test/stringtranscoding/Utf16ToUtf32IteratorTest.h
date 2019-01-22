#include "../gtest/include/gtest/gtest.h"
#include "../../package/src/stringtranscoding/stringtranscoding.h"

namespace {

class Utf16ToUtf32IteratorTest : public ::testing::Test {
protected:
    Utf16ToUtf32IteratorTest()
    { }

    ~Utf16ToUtf32IteratorTest() override
    { }

    void SetUp() override
    { }

    void TearDown() override
    { }
};

TEST_F(Utf16ToUtf32IteratorTest, IterateString) {
    std::u16string source = u"€,®T𐐷st𤭢";
    std::u32string expected = U"€,®T𐐷st𤭢";

    CREATE_ITER_U16_U32(begin, const char16_t *, source.c_str());
    CREATE_ITER_U16_U32(iterator, const char16_t *, begin);
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
