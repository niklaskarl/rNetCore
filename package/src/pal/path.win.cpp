#ifdef WIN32

#include "path.h"
#include "../stringtranscoding/stringtranscoding.h"

#include <shlwapi.h>

bool pal::path::is_absolute_path(const utf8char_t* path)
{
    utf16string path_u16 = utf8_to_utf16(path);
    return is_absolute_path(path_u16.c_str());
}

bool pal::path::is_absolute_path(const utf16char_t* path)
{
    return !::PathIsRelativeW((const wchar_t *)path);
}

bool pal::path::get_absolute_path(const utf8char_t *path, utf8string& absolute_path)
{
    absolute_path.clear();

    utf16string path_u16 = utf8_to_utf16(path);

    utf16string absolute_path_u16;
    bool result = get_absolute_path(path_u16.c_str(), absolute_path_u16);

    utf16_to_utf8(absolute_path_u16, absolute_path);
    return result;
}

bool pal::path::get_absolute_path(const utf16char_t *path, utf16string& absolute_path)
{
    absolute_path.clear();

    utf16char_t stackbuffer[MAX_PATH];
    DWORD capacity = MAX_PATH;
    utf16char_t *buffer = stackbuffer;
    DWORD length = ::GetFullPathNameW((const wchar_t *)path, capacity, (wchar_t *)buffer, NULL);
    while (length > capacity)
    {
        if (buffer != stackbuffer)
        {
            delete[] buffer;
        }

        capacity = length;
        buffer = new utf16char_t[capacity];
        length = ::GetFullPathNameW((const wchar_t *)path, capacity, (wchar_t *)buffer, NULL);
    }

    if (length > 0)
    {
        absolute_path.append(buffer, length);
    }

    if (buffer != stackbuffer)
    {
        delete[] buffer;
    }

    return length > 0;
}

bool pal::path::directory_exists(const utf8char_t* path)
{
    utf16string path_u16 = utf8_to_utf16(path);

    return directory_exists(path_u16.c_str());
}

bool pal::path::directory_exists(const utf16char_t* path)
{
    return ::PathIsDirectoryW((const wchar_t *)path);
}

bool pal::path::file_exists(const utf8char_t* path)
{
    utf16string path_u16 = utf8_to_utf16(path);

    return file_exists(path_u16.c_str());
}

bool pal::path::file_exists(const utf16char_t* path)
{
    return ::PathFileExistsW((const wchar_t *)path);
}

bool pal::path::find_file_on_path(const utf8char_t* filename, utf8string& absolute_path)
{
    absolute_path.clear();

    utf16string filename_u16 = utf8_to_utf16(filename);

    utf16string absolute_path_u16;
    bool result = find_file_on_path(filename_u16.c_str(), absolute_path_u16);

    utf16_to_utf8(absolute_path_u16, absolute_path);
    return result;
}

bool pal::path::find_file_on_path(const utf16char_t* filename, utf16string& absolute_path)
{
    absolute_path.clear();

    wchar_t buffer[MAX_PATH];
    if (wcscpy_s(buffer, (const wchar_t *)filename) != 0)
    {
        return false;
    }

    if (!::PathFindOnPathW(buffer, NULL))
    {
        return false;
    }

    absolute_path.append((const utf16char_t *)buffer);
    return true;
}

bool pal::path::list_directory(const utf8char_t *path, std::vector< utf8string > *files, std::vector< utf8string > *directories)
{
    std::vector< utf16string > files_u16;
    std::vector< utf16string > *pfiles_u16 = NULL;
    if (files)
    {
        files->clear();
        pfiles_u16 = &files_u16;
    }

    std::vector< utf16string > directories_u16;
    std::vector< utf16string > *pdirectories_u16 = NULL;
    if (directories)
    {
        directories->clear();
        pdirectories_u16 = &directories_u16;
    }

    utf16string path_u16 = utf8_to_utf16(path);

    utf16string absolute_path_u16;
    bool result = list_directory(path_u16.c_str(), pfiles_u16, pdirectories_u16);

    if (files)
    {
        for (std::vector< utf16string >::iterator iter = files_u16.begin(); iter != files_u16.end(); ++iter)
        {
            files->push_back(utf16_to_utf8(*iter));
        }
    }

    if (directories)
    {
        for (std::vector< utf16string >::iterator iter = directories_u16.begin(); iter != directories_u16.end(); ++iter)
        {
            directories->push_back(utf16_to_utf8(*iter));
        }
    }

    return result;
}

bool pal::path::list_directory(const utf16char_t *path, std::vector< utf16string > *files, std::vector< utf16string > *directories)
{
    if (files)
    {
        files->clear();
    }

    if (directories)
    {
        directories->clear();
    }

    utf16string search(path);
    search.append(_U16"\\*");

    WIN32_FIND_DATAW find_data;
    HANDLE handle = ::FindFirstFileW((const wchar_t *)search.c_str(), &find_data);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return ::GetLastError() == ERROR_FILE_NOT_FOUND;
    }

    do
    {
        if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            if (directories)
            {
                directories->push_back(utf16string((const utf16char_t *)find_data.cFileName));
            }
        }
        else
        {
            if (files)
            {
                files->push_back(utf16string((const utf16char_t *)find_data.cFileName));
            }
        }
    } while (::FindNextFileW(handle, &find_data));

    bool result = ::GetLastError() == ERROR_NO_MORE_FILES;
    ::FindClose(handle);

    return result;
}

#endif // WIN32
