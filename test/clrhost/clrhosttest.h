#ifndef __CLRHOST_TEST_H
#define __CLRHOST_TEST_H

#include "../gtest/include/gtest/gtest.h"
#include "../../../package/src/clrhost/clrhosting.h"

namespace {

    class ClrHostTest : public ::testing::Test {
    protected:
        ClrHostTest() {}

        ~ClrHostTest() override {}

        void SetUp() override {}

        void TearDown() override {}
    };

    typedef void (*delegate_ptr)();
    TEST_F(ClrHostTest, LoadCoreCLR) {

#ifdef WIN32
        const utf16char_t *app_domain_dll = u"..\\..\\..\\test\\clrhost\\TestAssembly\\bin\\Debug\\netcoreapp2.2\\publish\\TestAssembly.dll";
#elif unix
        const utf16char_t *app_domain_dll = u"../../../test/clrhost/TestAssembly/bin/Debug/netcoreapp2.2/publish/TestAssembly.dll";
#endif

        utf16string directory = find_dotnet_core_absolute_directory();
        std::vector<SemVer> versions = get_installed_dotnet_core_versions(directory.c_str());
        ASSERT_GT(versions.size(), 0);
        clr_host host = load_dotnet_core_runtime(directory.c_str(), versions[versions.size() - 1]);

        clr_app_domain app_domain = host.create_app_domain(app_domain_dll);
        delegate_ptr delegate = (delegate_ptr)app_domain.create_delegate(u"TestAssembly, Version=1.0.0.0", u"TestAssembly.TestClass", u"HelloWorld");
        // delegate();
    }

} // namespace

#endif // __CLRHOST_TEST_H
