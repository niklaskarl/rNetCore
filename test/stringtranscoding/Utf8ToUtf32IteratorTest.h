#include "../gtest/include/gtest/gtest.h"
#include "../../package/src/stringtranscoding/stringtranscoding.h"

namespace {

class Utf8ToUtf32IteratorTest : public ::testing::Test {
protected:
    Utf8ToUtf32IteratorTest()
    { }

    ~Utf8ToUtf32IteratorTest() override
    { }

    void SetUp() override
    { }

    void TearDown() override
    { }
};

TEST_F(Utf8ToUtf32IteratorTest, IterateString) {
    std::string source = u8"€,®T𐐷st𤭢";
    std::u32string expected = U"€,®T𐐷st𤭢";

    CREATE_ITER_U8_U32(begin, const char *, source.c_str());
    CREATE_ITER_U8_U32(iterator, const char *, begin);
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
