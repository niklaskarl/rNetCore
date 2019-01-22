#ifndef __CLRHOST_APPDOMAIN_DOTNET_H__
#define __CLRHOST_APPDOMAIN_DOTNET_H__

#include "../abstraction.h"
#include "../comptr.h"

#include "../include/mscoree.h"

#include <memory>

class AppDomainDotnet : public IAppDomain
{
public:
    AppDomainDotnet(const std::shared_ptr<ICLRHost> &pHost, ICLRRuntimeHost2 *pRuntimeHost, uint32_t domainId);
    ~AppDomainDotnet();

    AppDomainDotnet(const AppDomainDotnet &other) = delete;
    AppDomainDotnet(AppDomainDotnet &&other) noexcept = delete;

    AppDomainDotnet& operator=(const AppDomainDotnet& other) = delete;
    AppDomainDotnet& operator=(AppDomainDotnet&& other) noexcept = delete;

public:
    HRESULT CreateDelegate(const utf16char_t *pAssemblyName, const utf16char_t *pTypeName, const utf16char_t *pMethodName, void **ppDelegate);

private:
    std::shared_ptr<ICLRHost> m_pHost;
    ComPtr<ICLRRuntimeHost2> m_pRuntimeHost;
    uint32_t m_domainId;
};

#endif // __CLRHOST_APPDOMAIN_DOTNET_H__
