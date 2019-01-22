#include "clrhosting.h"
#include "dotnet/win/clrhost_dotnet_win.h"
#include "dotnet/unix/clrhost_dotnet_unix.h"

#include "include/hresult.h"

#include "../stringtranscoding/stringtranscoding.h"
#include "../pal/path.h"
#include "../pal/linking.h"

#include <algorithm>
#include <set>

#ifdef WIN32
#define DOTNET_EXECUTABLE_FILENAME _U16"dotnet.exe"
#define CORECLR_LIBRARY_FILENAME _U16"coreclr.dll"
#define CORECLR_ARG_DELIMITER _U16";"
#endif

#ifdef unix
#define DOTNET_EXECUTABLE_FILENAME _U16"dotnet"
#define CORECLR_LIBRARY_FILENAME _U16"libcoreclr.so"
#define CORECLR_ARG_DELIMITER _U16":"
#endif

utf16string find_dotnet_core_absolute_directory()
{
    utf16string executable;
    if (!pal::path::find_file_on_path(DOTNET_EXECUTABLE_FILENAME, executable))
    {
        throw hresult_error(E_FAIL);
    }

    utf16string directory;
    if (!pal::path::get_parent_directory(executable.c_str(), directory))
    {
        throw hresult_error(E_FAIL);
    }

    return directory;
}

std::vector< SemVer > get_installed_dotnet_core_versions(const utf16char_t *directory)
{
    const utf16char_t *path = _U16"shared" PLATFORM_PATH_DELIMITER_STRING "Microsoft.NETCore.App";

    utf16string version_directory;
    pal::path::join_path(directory, path, version_directory);

    if (!pal::path::directory_exists(version_directory.c_str()))
    {
        throw hresult_error(E_FAIL);
    }

    std::vector< utf16string > directories;
    pal::path::list_directory(version_directory.c_str(), NULL, &directories);

    std::vector< SemVer > versions;
    versions.reserve(directories.size());
    utf8string version_text;
    SemVer current;
    for (size_t i = 0; i < directories.size(); ++i)
    {
        version_text.clear();
        utf16_to_utf8(directories[i], version_text);
        if (SemVer::TryParse(version_text.c_str(), current))
        {
            versions.push_back(current);
        }
    }

    std::sort(versions.begin(), versions.end());

    return versions;
}

clr_host load_dotnet_core_runtime(const utf16char_t *directory, const SemVer &version)
{
    utf16string version_text = utf8_to_utf16(version.ToString());

    utf16string sub(_U16"shared" PLATFORM_PATH_DELIMITER_STRING "Microsoft.NETCore.App" PLATFORM_PATH_DELIMITER_STRING);
    sub.append(version_text);

    utf16string coreclr_directory;
    pal::path::join_path(directory, sub.c_str(), coreclr_directory);

    utf16string coreclr_library;
    pal::path::join_path(coreclr_directory.c_str(), CORECLR_LIBRARY_FILENAME, coreclr_library);

    return load_core_clr(coreclr_library.c_str());
}

clr_host load_core_clr(const utf16char_t *path)
{
    HRESULT hr = S_OK;
    std::shared_ptr<ICLRHost> host = NULL;

#ifdef WIN32
    hr = CLRHostDotnetWin::Create(path, &host);
#else
    hr = CLRHostDotnetUnix::Create(path, &host);
#endif

    if (FAILED(hr))
    {
        throw hresult_error(hr);
    }

    return clr_host(host);
}
