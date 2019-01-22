#ifndef __PAL_LINKING_H__
#define __PAL_LINKING_H__

#include "../stringtranscoding/stringtranscoding.h"

namespace pal
{
namespace linking
{

    void *open_library(const utf8char_t *path);
    void *open_library(const utf16char_t *path);

    void *locate_function(void *library, const utf8char_t *function_name);
    void *locate_function(void *library, const utf16char_t *function_name);

    bool close_library(void *library);

}
}

#endif // __PAL_LINKING_H__
