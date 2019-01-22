#include "../gtest/include/gtest/gtest.h"
#include "../../package/src/stringtranscoding/stringtranscoding.h"

namespace {

class Utf8ToUtf16IteratorTest : public ::testing::Test {
protected:
    Utf8ToUtf16IteratorTest()
    { }

    ~Utf8ToUtf16IteratorTest() override
    { }

    void SetUp() override
    { }

    void TearDown() override
    { }
};

TEST_F(Utf8ToUtf16IteratorTest, IterateString) {
    std::string source = u8"€,®T𐐷st𤭢";
    std::u16string expected = u"€,®T𐐷st𤭢";

    CREATE_ITER_U8_U16(begin, const char *, source.c_str());
    CREATE_ITER_U8_U16(iterator, const char *, source.c_str());
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
