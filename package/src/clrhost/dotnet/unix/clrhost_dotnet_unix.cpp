#include "clrhost_dotnet_unix.h"

#include "../tpalist_builder.h"
#include "../appdomain_dotnet.h"

#include "../../../pal/path.h"
#include "../../../pal/linking.h"

CLRHostDotnetUnix::CLRHostDotnetUnix() :
    m_pWeakThis(),
    m_coreCLRPath(),
    m_pCoreCLRLibrary(NULL),
    m_pInitialize(NULL)
{ }

CLRHostDotnetUnix::~CLRHostDotnetUnix()
{
    if (m_pCoreCLRLibrary)
    {
        pal::linking::close_library(m_pCoreCLRLibrary);
    }
}

HRESULT CLRHostDotnetUnix::Create(const utf16char_t *pDllPath, std::shared_ptr<ICLRHost> *ppHost)
{
    std::shared_ptr<CLRHostDotnetUnix> pInstance = std::make_shared<CLRHostDotnetUnix>();

    HRESULT hr = ppHost ? S_OK : E_POINTER;
    if (SUCCEEDED(hr))
    {
        pInstance->m_coreCLRPath = utf16_to_utf8(pDllPath);
        pInstance->m_pCoreCLRLibrary = pal::linking::open_library(pInstance->m_coreCLRPath.c_str());
        hr = pInstance->m_pCoreCLRLibrary ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        pInstance->m_pInitialize = (coreclr_initialize_ptr)pal::linking::locate_function(pInstance->m_pCoreCLRLibrary, "coreclr_initialize");
        hr = pInstance->m_pInitialize ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        pInstance->m_pWeakThis = pInstance;
        *ppHost = std::move(pInstance);
    }

    return hr;
}

HRESULT CLRHostDotnetUnix::CreateAppDomain(const utf16char_t *pDllPath, std::shared_ptr<IAppDomain> *ppDomain)
{
    ICLRRuntimeHost2 *pRuntimeHost;
    uint32_t domainId;

    HRESULT hr = ppDomain ? S_OK : E_POINTER;

    utf8string dllPath = utf16_to_utf8(pDllPath);
    utf8string dllAbsolutePath;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_absolute_path(dllPath.c_str(), dllAbsolutePath) ? S_OK : E_FAIL;
    }

    utf8string dllDirectory;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_parent_directory(dllAbsolutePath.c_str(), dllDirectory) ? S_OK : E_FAIL;
    }

    utf8string coreCLRDirectory;
    if (SUCCEEDED(hr))
    {
        hr = pal::path::get_parent_directory(m_coreCLRPath.c_str(), coreCLRDirectory) ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        utf8string nativeDllSearchDirs(dllDirectory);
        nativeDllSearchDirs.append(1, ':');
        nativeDllSearchDirs.append(coreCLRDirectory);

        const utf8char_t *const extensions[] = {
            ".ni.dll",
            ".dll",
            ".ni.exe",
            ".exe"
        };

        tpalist_builder<utf8char_t> tpalist(':');
        tpalist.add_file(dllAbsolutePath.c_str());
        tpalist.add_directory(coreCLRDirectory.c_str(), extensions, sizeof(extensions) / sizeof(*extensions));

        const char *keys[] = {
            "TRUSTED_PLATFORM_ASSEMBLIES",
            "APP_PATHS",
            "APP_NI_PATHS",
            "NATIVE_DLL_SEARCH_DIRECTORIES",
            "System.GC.Server",
            "System.Globalization.Invariant"
        };

        const char *values[] = {
            tpalist.c_str(),
            dllDirectory.c_str(),
            dllDirectory.c_str(),
            nativeDllSearchDirs.c_str(),
            "false",
            "false"
        };

        hr = m_pInitialize(
            dllAbsolutePath.c_str(),
            "Sample Host AppDomain",
            sizeof(keys) / sizeof(keys[0]),
            keys,
            values,
            (void **)&pRuntimeHost,
            &domainId
            );
    }

    if (SUCCEEDED(hr))
    {
        *ppDomain = std::make_shared<AppDomainDotnet>(std::shared_ptr<ICLRHost>(m_pWeakThis), pRuntimeHost, domainId);
    }

    return hr;
}
