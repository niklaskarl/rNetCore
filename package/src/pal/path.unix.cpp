#ifdef unix

#include "path.h"

#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

bool pal::path::is_absolute_path(const utf8char_t* path)
{
    char buffer[PATH_MAX];
    const char* realPath = realpath(path, buffer);
    return realPath && *realPath == '/';
}

bool pal::path::is_absolute_path(const utf16char_t* path)
{
    return is_absolute_path(utf16_to_utf8(path).c_str());
}

bool pal::path::get_absolute_path(const utf8char_t* path, utf8string& absolute_path)
{
    bool result = false;

    absolute_path.clear();

    char buffer[PATH_MAX];
    const char* real_path;
    if ((real_path = realpath(path, buffer)) != NULL && *real_path != '\0')
    {
        absolute_path.assign(real_path);
        // realpath should return canonicalized path without the trailing slash
        assert(absolute_path.back() != '/');

        result = true;
    }

    return result;
}

bool pal::path::get_absolute_path(const utf16char_t* path, utf16string& absolute_path)
{
    absolute_path.clear();

    utf8string path_u8 = utf16_to_utf8(path);

    utf8string absolute_path_u8;
    bool result = get_absolute_path(path_u8.c_str(), absolute_path_u8);

    utf8_to_utf16(absolute_path_u8, absolute_path);
    return result;
}

bool pal::path::directory_exists(const utf8char_t* path)
{
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

bool pal::path::directory_exists(const utf16char_t* path)
{
    utf8string path_u8 = utf16_to_utf8(path);
    return directory_exists(path_u8.c_str());
}

bool pal::path::file_exists(const utf8char_t* path)
{
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISREG(sb.st_mode);
}

bool pal::path::file_exists(const utf16char_t* path)
{
    utf8string path_u8 = utf16_to_utf8(path);
    return file_exists(path_u8.c_str());
}

bool pal::path::find_file_on_path(const utf8char_t* filename, utf8string& absolute_path)
{
    absolute_path.clear();

    bool result = false;

    char* path = getenv("PATH");
    char* p = path;

    struct stat sb;

    if (path)
    {
        while (*p != '\0' && !result)
        {
            if (*p == ':')
            {
                std::string file(path, p - path);
                if (file.back() != '/')
                {
                    file.append("/");
                }

                file.append(filename);

                if (stat(file.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
                {
                    result = get_absolute_path(file.c_str(), absolute_path);
                }

                path = p + 1;
            }

            p++;
        }
    }

    return result;
}

bool pal::path::find_file_on_path(const utf16char_t* filename, utf16string& absolute_path)
{
    absolute_path.clear();

    utf8string filename_u8 = utf16_to_utf8(filename);

    utf8string absolute_path_u8;
    bool result = find_file_on_path(filename_u8.c_str(), absolute_path_u8);

    utf8_to_utf16(absolute_path_u8, absolute_path);
    return result;
}

bool pal::path::list_directory(const utf8char_t *path, std::vector< utf8string > *files, std::vector< utf8string > *directories)
{
    if (files)
    {
        files->clear();
    }

    if (directories)
    {
        directories->clear();
    }

    DIR *dir = opendir(path);
    if (!dir)
    {
        return false;
    }

    std::string filename;
    std::string absolutePath;
    struct stat sb;
    for (dirent *entry = readdir(dir); entry != NULL; entry = readdir(dir))
    {
        absolutePath.clear();
        if (join_path(path, entry->d_name, absolutePath))
        {
            if (stat(absolutePath.c_str(), &sb) == 0)
            {
                if (S_ISREG(sb.st_mode) && files)
                {
                    filename.assign(entry->d_name);
                    files->push_back(filename);
                }
                else if (S_ISDIR(sb.st_mode) && directories)
                {
                    filename.assign(entry->d_name);
                    directories->push_back(filename);
                }
            }
        }
    }

    closedir(dir);
    return true;
}

bool pal::path::list_directory(const utf16char_t *path, std::vector< utf16string > *files, std::vector< utf16string > *directories)
{
    std::vector< utf8string > files_u8;
    std::vector< utf8string > *pfiles_u8 = NULL;
    if (files)
    {
        files->clear();
        pfiles_u8 = &files_u8;
    }

    std::vector< utf8string > directories_u8;
    std::vector< utf8string > *pdirectories_u8 = NULL;
    if (directories)
    {
        directories->clear();
        pdirectories_u8 = &directories_u8;
    }

    utf8string path_u8 = utf16_to_utf8(path);

    utf8string absolute_path_u8;
    bool result = list_directory(path_u8.c_str(), pfiles_u8, pdirectories_u8);

    if (files)
    {
        for (std::vector< utf8string >::iterator iter = files_u8.begin(); iter != files_u8.end(); ++iter)
        {
            files->push_back(utf8_to_utf16(*iter));
        }
    }

    if (directories)
    {
        for (std::vector< utf8string >::iterator iter = directories_u8.begin(); iter != directories_u8.end(); ++iter)
        {
            directories->push_back(utf8_to_utf16(*iter));
        }
    }

    return result;
}

#endif // unix
