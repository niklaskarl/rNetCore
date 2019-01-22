#ifndef __CLRHOST_CLRHOST_DOTNET_H__
#define __CLRHOST_CLRHOST_DOTNET_H__

#include "../../abstraction.h"
#include "../../include/unixhost.h"

#include <memory>

class CLRHostDotnetUnix : public ICLRHost
{
public:
    static HRESULT Create(const utf16char_t *pDllPath, std::shared_ptr<ICLRHost> *ppHost);

    CLRHostDotnetUnix();
    CLRHostDotnetUnix(const CLRHostDotnetUnix &other) = delete;
    CLRHostDotnetUnix(CLRHostDotnetUnix &&other) noexcept = delete;

    ~CLRHostDotnetUnix();

    CLRHostDotnetUnix& operator=(const CLRHostDotnetUnix& other) = delete;
    CLRHostDotnetUnix& operator=(CLRHostDotnetUnix&& other) noexcept = delete;

public:
    HRESULT CreateAppDomain(const utf16char_t *pDllPath, std::shared_ptr<IAppDomain> *ppDomain);

private:
    std::weak_ptr<CLRHostDotnetUnix> m_pWeakThis;

    utf8string m_coreCLRPath;
    void *m_pCoreCLRLibrary;
    coreclr_initialize_ptr m_pInitialize;
};

#endif // __CLRHOST_CLRHOST_DOTNET_H__
