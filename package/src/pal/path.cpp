#include "path.h"

bool pal::path::join_path(const utf8char_t* path1, const utf8char_t* path2, utf8string& joined_path)
{
    bool result = false;

    joined_path.clear();
    if (path1 && path2)
    {
        if (!is_absolute_path(path2))
        {
            joined_path.assign(path1);
        }

        if (joined_path.length() && joined_path.at(joined_path.length() - 1) != PLATFORM_PATH_DELIMITER)
        {
            joined_path.append(1, PLATFORM_PATH_DELIMITER);
        }

        joined_path.append(path2);

        result = true;
    }
    else if (path1)
    {
        joined_path.assign(path1);
        result = true;
    }
    else if (path2)
    {
        joined_path.assign(path2);
        result = true;
    }

    return result;
}

bool pal::path::join_path(const utf16char_t* path1, const utf16char_t* path2, utf16string& joined_path)
{
    bool result = false;

    joined_path.clear();
    if (path1 && path2)
    {
        if (!is_absolute_path(path2))
        {
            joined_path.assign(path1);
        }

        if (joined_path.length() && joined_path.at(joined_path.length() - 1) != PLATFORM_PATH_DELIMITER)
        {
            joined_path.append(1, PLATFORM_PATH_DELIMITER);
        }

        joined_path.append(path2);

        result = true;
    }
    else if (path1)
    {
        joined_path.assign(path1);
        result = true;
    }
    else if (path2)
    {
        joined_path.assign(path2);
        result = true;
    }

    return result;
}

bool pal::path::get_parent_directory(const utf8char_t* path, utf8string &directory)
{
    directory.clear();

    const utf8char_t *filename;
    if (!pal::path::get_file_name(path, &filename))
    {
        return false;
    }

    directory.assign(path, filename - path - 1);
    return true;
}

bool pal::path::get_parent_directory(const utf16char_t* path, utf16string &directory)
{
    directory.clear();

    const utf16char_t *filename;
    if (!pal::path::get_file_name(path, &filename))
    {
        return false;
    }

    directory.assign(path, filename - path - 1);
    return true;
}

bool pal::path::get_file_name(const utf8char_t *path, const utf8char_t **filename)
{
    *filename = path;
    while (*path)
    {
        if (*path == PLATFORM_PATH_DELIMITER)
        {
            *filename = path + 1;
        }

        ++path;
    }

    return *filename != path;
}

bool pal::path::get_file_name(const utf16char_t *path, const utf16char_t **filename)
{
    *filename = path;
    while (*path)
    {
        if (*path == PLATFORM_PATH_DELIMITER)
        {
            *filename = path + 1;
        }

        ++path;
    }

    return *filename != path;
}
