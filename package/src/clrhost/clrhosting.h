#ifndef __CLRHOST_CLRHOSTING_H__
#define __CLRHOST_CLRHOSTING_H__

#include "include/hresult.h"

#include "../stringtranscoding/stringtranscoding.h"
#include "../semver/semanticversion.h"

#include <memory>
#include <string>
#include <vector>

class ICLRHost;
class IAppDomain;

class clr_host;
class clr_app_domain;

class clr_host
{
public:
    clr_host(const std::shared_ptr<ICLRHost> host);
    clr_host(const clr_host &other) = delete;
    clr_host(clr_host &&other) noexcept;
    ~clr_host();

    clr_host& operator=(const clr_host& other) = delete;
    clr_host& operator=(clr_host&& other) noexcept;

public:
    clr_app_domain create_app_domain(const utf16char_t *dll_path);

private:
    std::shared_ptr<ICLRHost> m_host;
};

class clr_app_domain
{
public:
    clr_app_domain(const std::shared_ptr<IAppDomain> &domain);
    clr_app_domain(const clr_app_domain &other) = delete;
    clr_app_domain(clr_app_domain &&other) noexcept;
    ~clr_app_domain();

    clr_app_domain& operator=(const clr_app_domain& other) = delete;
    clr_app_domain& operator=(clr_app_domain&& other) noexcept;
    
public:
    void *create_delegate(const utf16char_t *assembly, const utf16char_t *type, const utf16char_t *method);

private:
    std::shared_ptr<IAppDomain> m_domain;
};

utf16string find_dotnet_core_absolute_directory();
std::vector< SemVer > get_installed_dotnet_core_versions(const utf16char_t *directory);

clr_host load_dotnet_core_runtime(const utf16char_t *directory, const SemVer &version);
clr_host load_core_clr(const utf16char_t *path);

#endif // __CLRHOST_CLRHOSTING_H__
