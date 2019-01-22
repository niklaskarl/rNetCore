#ifndef __CLRHOST_CLRHOST_DOTNET_WIN_H__
#define __CLRHOST_CLRHOST_DOTNET_WIN_H__

#include "../../abstraction.h"
#include "../../comptr.h"

#include "../../include/mscoree.h"

#include <memory>

class CLRHostDotnetWin : public ICLRHost
{
public:
    static HRESULT Create(const utf16char_t *pDllPath, std::shared_ptr<ICLRHost> *ppHost);

    CLRHostDotnetWin();
    CLRHostDotnetWin(const CLRHostDotnetWin &other) = delete;
    CLRHostDotnetWin(CLRHostDotnetWin &&other) noexcept = delete;

    ~CLRHostDotnetWin();

    CLRHostDotnetWin& operator=(const CLRHostDotnetWin& other) = delete;
    CLRHostDotnetWin& operator=(CLRHostDotnetWin&& other) noexcept = delete;

public:
    HRESULT CreateAppDomain(const utf16char_t *pDllPath, std::shared_ptr<IAppDomain> *ppDomain);

private:
    std::weak_ptr<CLRHostDotnetWin> m_pWeakThis;

    utf16string m_coreCLRPath;
    void * m_pCoreCLRLibrary;

    ComPtr<ICLRRuntimeHost2> m_pRuntimeHost;
    bool m_started;
};

#endif // __CLRHOST_CLRHOST_DOTNET_WIN_H__
