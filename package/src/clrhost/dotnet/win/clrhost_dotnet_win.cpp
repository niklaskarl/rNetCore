#include "clrhost_dotnet_win.h"

#include "../tpalist_builder.h"
#include "../appdomain_dotnet.h"

#include "../../../pal/path.h"
#include "../../../pal/linking.h"

#include <set>
#include <string>
#include <vector>

CLRHostDotnetWin::CLRHostDotnetWin() :
    m_pWeakThis(),
    m_coreCLRPath(),
    m_pCoreCLRLibrary(nullptr),
    m_pRuntimeHost(nullptr),
    m_started(false)
{ }

CLRHostDotnetWin::~CLRHostDotnetWin()
{
    if (m_pRuntimeHost)
    {
        if (m_started)
        {
            m_pRuntimeHost->Stop();
        }

        m_pRuntimeHost = NULL;
    }

    if (m_pCoreCLRLibrary)
    {
        pal::linking::close_library(m_pCoreCLRLibrary);
        m_pCoreCLRLibrary = NULL;
    }
}

HRESULT CLRHostDotnetWin::Create(const utf16char_t *pDllPath, std::shared_ptr<ICLRHost> *ppHost)
{
    std::shared_ptr<CLRHostDotnetWin> pInstance = std::make_shared<CLRHostDotnetWin>();

    FnGetCLRRuntimeHost pGetCLRRuntimeHost;
    HRESULT hr = ppHost ? S_OK : E_POINTER;
    if (SUCCEEDED(hr))
    {
        pInstance->m_coreCLRPath.assign(pDllPath);
        pInstance->m_pCoreCLRLibrary = pal::linking::open_library(pDllPath);
        hr = pInstance->m_pCoreCLRLibrary ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        pGetCLRRuntimeHost = (FnGetCLRRuntimeHost)pal::linking::locate_function(pInstance->m_pCoreCLRLibrary, "GetCLRRuntimeHost");
        hr = pGetCLRRuntimeHost ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        hr = pGetCLRRuntimeHost(IID_ICLRRuntimeHost2, (IUnknown **)&pInstance->m_pRuntimeHost);
    }

    if (SUCCEEDED(hr))
    {
        hr = pInstance->m_pRuntimeHost->SetStartupFlags(
            // These startup flags control runtime-wide behaviors.
            // A complete list of STARTUP_FLAGS can be found in mscoree.h,
            // but some of the more common ones are listed below.
            static_cast<STARTUP_FLAGS>(
                // STARTUP_FLAGS::STARTUP_SERVER_GC |                               // Use server GC
                // STARTUP_FLAGS::STARTUP_LOADER_OPTIMIZATION_MULTI_DOMAIN |        // Maximize domain-neutral loading
                // STARTUP_FLAGS::STARTUP_LOADER_OPTIMIZATION_MULTI_DOMAIN_HOST |   // Domain-neutral loading for strongly-named assemblies
                STARTUP_FLAGS::STARTUP_CONCURRENT_GC |                              // Use concurrent GC
                STARTUP_FLAGS::STARTUP_SINGLE_APPDOMAIN |                           // All code executes in the default AppDomain 
                                                                                    // (required to use the runtimeHost->ExecuteAssembly helper function)
                STARTUP_FLAGS::STARTUP_LOADER_OPTIMIZATION_SINGLE_DOMAIN            // Prevents domain-neutral loading
            )
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = pInstance->m_pRuntimeHost->Start();
    }

    if (SUCCEEDED(hr))
    {
        pInstance->m_started = true;

        pInstance->m_pWeakThis = pInstance;
        *ppHost = std::move(pInstance);
    }

    return hr;
}

HRESULT CLRHostDotnetWin::CreateAppDomain(const utf16char_t *pDllPath, std::shared_ptr<IAppDomain> *ppDomain)
{
    DWORD domainId;

    int appdomainFlags =
        // APPDOMAIN_FORCE_TRIVIAL_WAIT_OPERATIONS |        // Do not pump messages during wait
        // APPDOMAIN_SECURITY_SANDBOXED |                   // Causes assemblies not from the TPA list to be loaded as partially trusted
        APPDOMAIN_ENABLE_PLATFORM_SPECIFIC_APPS |           // Enable platform-specific assemblies to run
        APPDOMAIN_ENABLE_PINVOKE_AND_CLASSIC_COMINTEROP |   // Allow PInvoking from non-TPA assemblies
        APPDOMAIN_DISABLE_TRANSPARENCY_ENFORCEMENT;         // Entirely disables transparency checks

    HRESULT hr = ppDomain ? S_OK : E_POINTER;

    utf16string dllAbsolutePath;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_absolute_path(pDllPath, dllAbsolutePath) ? S_OK : E_FAIL;
    }

    utf16string dllDirectory;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_parent_directory(dllAbsolutePath.c_str(), dllDirectory) ? S_OK : E_FAIL;
    }

    utf16string coreclrDirectory;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_parent_directory(m_coreCLRPath.c_str(), coreclrDirectory) ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        utf16string nativeDllSearchDirs(dllDirectory);
        nativeDllSearchDirs.append(u";");
        nativeDllSearchDirs.append(coreclrDirectory);

        const utf16char_t *const extensions[] = {
            u".ni.dll",
            u".dll",
            u".ni.exe",
            u".exe"
        };

        tpalist_builder<utf16char_t> tpalist(u';');
        tpalist.add_file(dllAbsolutePath.c_str());
        tpalist.add_directory(coreclrDirectory.c_str(), extensions, sizeof(extensions) / sizeof(*extensions));

        const utf16char_t *keys[] = {
            u"TRUSTED_PLATFORM_ASSEMBLIES",
            u"APP_PATHS",
            u"APP_NI_PATHS",
            u"NATIVE_DLL_SEARCH_DIRECTORIES"
        };

        const utf16char_t *values[] = {
            tpalist.c_str(),
            dllDirectory.c_str(),
            dllDirectory.c_str(),
            nativeDllSearchDirs.c_str()
        };

        hr = m_pRuntimeHost->CreateAppDomainWithManager(
            u"Sample Host AppDomain",       // Friendly AD name
            appdomainFlags,
            NULL,                           // Optional AppDomain manager assembly name
            NULL,                           // Optional AppDomain manager type (including namespace)
            sizeof(keys)/sizeof(keys[0]),
            keys,
            values,
            &domainId);
    }

    if (SUCCEEDED(hr))
    {
        *ppDomain = std::make_shared<AppDomainDotnet>(std::shared_ptr<ICLRHost>(m_pWeakThis), m_pRuntimeHost, domainId);
    }

    return hr;
}
