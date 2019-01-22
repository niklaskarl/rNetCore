#ifdef unix

#include "linking.h"

#include <dlfcn.h>

void *pal::linking::open_library(const utf8char_t *path)
{
    return dlopen(path, RTLD_NOW);
}

void *pal::linking::open_library(const utf16char_t *path)
{
    utf8string path_u8 = utf16_to_utf8(path);
    return open_library(path_u8.c_str());
}

void *pal::linking::locate_function(void *library, const utf8char_t *function_name)
{
    return dlsym(library, function_name);
}

void *pal::linking::locate_function(void *library, const utf16char_t *function_name)
{
    utf8string function_name_u8 = utf16_to_utf8(function_name);
    return locate_function(library, function_name_u8.c_str());
}

bool pal::linking::close_library(void *library)
{
    return dlclose(library) == 0;
}

#endif // unix
