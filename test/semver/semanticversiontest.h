#ifndef __SEMANTICVERSION_TEST_H
#define __SEMANTICVERSION_TEST_H

#include "../gtest/include/gtest/gtest.h"
#include "../../../package/src/semver/semanticversion.h"

namespace {

    class semver_test : public ::testing::Test {
    protected:
        semver_test() {}

        ~semver_test() override {}

        void SetUp() override {}

        void TearDown() override {}
    };

    TEST_F(semver_test, TryParse) {
        std::string text = "2.4.9-beta1.2+123321";
        SemVer version;

        ASSERT_TRUE(SemVer::TryParse(text.c_str(), version));
        ASSERT_EQ(version.GetMajor(), 2);
        ASSERT_EQ(version.GetMinor(), 4);
        ASSERT_EQ(version.GetPatch(), 9);
        ASSERT_STREQ(version.GetPreRelease().c_str(), "beta1.2");
        ASSERT_STREQ(version.GetBuildMetadata().c_str(), "123321");

        ASSERT_EQ(version.ToString(), text);
    }

} // namespace

#endif // __SEMANTICVERSION_TEST_H
