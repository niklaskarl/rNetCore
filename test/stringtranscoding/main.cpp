#include "../gtest/include/gtest/gtest.h"
#include "Utf8ToUtf32IteratorTest.h"
#include "Utf16ToUtf32IteratorTest.h"
#include "Utf32ToUtf8IteratorTest.h"
#include "Utf32ToUtf16IteratorTest.h"
#include "Utf8ToUtf16IteratorTest.h"
#include "Utf16ToUtf8IteratorTest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
