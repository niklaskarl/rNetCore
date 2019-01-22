#ifndef __PAL_PATH_H__
#define __PAL_PATH_H__

#include "../stringtranscoding/stringtranscoding.h"

#include <vector>
#include <string>

#ifdef WIN32
#define PLATFORM_PATH_DELIMITER '\\'
#define PLATFORM_PATH_DELIMITER_STRING "\\"
#endif // WIN32

#ifdef unix
#define PLATFORM_PATH_DELIMITER '/'
#define PLATFORM_PATH_DELIMITER_STRING "/"
#endif // unix

namespace pal
{
namespace path
{

bool join_path(const utf8char_t *path1, const utf8char_t *path2, utf8string &joinedPath);
bool join_path(const utf16char_t *path1, const utf16char_t *path2, utf16string &joinedPath);

bool is_absolute_path(const utf8char_t *path);
bool is_absolute_path(const utf16char_t *path);

bool get_absolute_path(const utf8char_t *path, utf8string &absolutePath);
bool get_absolute_path(const utf16char_t *path, utf16string &absolutePath);

bool directory_exists(const utf8char_t *path);
bool directory_exists(const utf16char_t *path);

bool file_exists(const utf8char_t *path);
bool file_exists(const utf16char_t *path);

bool find_file_on_path(const utf8char_t *fileName, utf8string &absolutePath);
bool find_file_on_path(const utf16char_t *fileName, utf16string &absolutePath);

bool get_parent_directory(const utf8char_t *path, utf8string &directory);
bool get_parent_directory(const utf16char_t *path, utf16string &directory);

bool get_file_name(const utf8char_t *path, const utf8char_t **fileName);
bool get_file_name(const utf16char_t *path, const utf16char_t **fileName);

bool list_directory(const utf8char_t *path, std::vector< utf8string > *files, std::vector< utf8string > *directories);
bool list_directory(const utf16char_t *path, std::vector< utf16string > *files, std::vector< utf16string > *directories);

}
}

#endif // __PAL_PATH_H__
