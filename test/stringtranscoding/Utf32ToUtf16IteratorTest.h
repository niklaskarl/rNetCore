#include "../gtest/include/gtest/gtest.h"
#include "../../package/src/stringtranscoding/stringtranscoding.h"

namespace {

class Utf32ToUtf16IteratorTest : public ::testing::Test {
protected:
    Utf32ToUtf16IteratorTest()
    { }

    ~Utf32ToUtf16IteratorTest() override
    { }

    void SetUp() override
    { }

    void TearDown() override
    { }
};

TEST_F(Utf32ToUtf16IteratorTest, IterateString) {
    std::u32string source = U"€,®T𐐷st𤭢";
    std::u16string expected = u"€,®T𐐷st𤭢";

    CREATE_ITER_U32_U16(begin, const char32_t *, source.c_str());
    CREATE_ITER_U32_U16(iterator, const char32_t *, begin);
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
