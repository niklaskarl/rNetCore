#ifdef WIN32

#include "linking.h"

#include <windows.h>

void *pal::linking::open_library(const utf8char_t *path)
{
    return open_library(utf8_to_utf16(path).c_str());
}

void *pal::linking::open_library(const utf16char_t *path)
{
    return ::LoadLibraryW((const wchar_t *)path);
}

void *pal::linking::locate_function(void *library, const utf8char_t *function_name)
{
    return (void *)::GetProcAddress((HMODULE)library, function_name);
}

void *pal::linking::locate_function(void *library, const utf16char_t *function_name)
{
    return locate_function(library, utf16_to_utf8(function_name).c_str());
}

bool pal::linking::close_library(void *library)
{
    return ::FreeLibrary((HMODULE)library);
}

#endif // Win32
