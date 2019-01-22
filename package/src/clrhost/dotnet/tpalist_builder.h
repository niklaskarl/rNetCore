#ifndef __CLRHOST_TPALIST_BUILDER_H__
#define __CLRHOST_TPALIST_BUILDER_H__

#include "../../pal/path.h"

#include <set>
#include <string>

template<typename TChar>
class tpalist_builder
{
public:
    tpalist_builder(TChar delimiter) :
        m_delimiter(delimiter),
        m_list(),
        m_added()
    { }

    void add_file(const TChar *file_path)
    {
        m_list.append(file_path);
        m_list.append(1, m_delimiter);
    }

    void add_directory(const TChar *directory_path, const TChar *const *extensions, size_t extensions_size)
    {
        std::vector< std::basic_string<TChar> > files;
        if (!pal::path::list_directory(directory_path, &files, NULL))
        {
            return;
        }

        for (size_t ext_index = 0; ext_index < extensions_size; ++ext_index)
        {
            std::basic_string<TChar> extension(extensions[ext_index]);
            for (size_t file_index = 0; file_index < files.size(); ++file_index)
            {
                const std::basic_string<TChar> &filename = files[file_index];
                ptrdiff_t ext_pos = filename.length() - extension.length();
                if (ext_pos > 0 && filename.compare(ext_pos, extension.length(), extension) == 0)
                {
                    std::basic_string<TChar> filename_without_ext(filename.substr(0, ext_pos));
                    if (m_added.find(filename_without_ext) == m_added.end())
                    {
                        m_added.insert(filename_without_ext);

                        filename_without_ext.clear();
                        pal::path::join_path(directory_path, filename.c_str(), filename_without_ext);
                        m_list.append(filename_without_ext);
                        m_list.append(1, m_delimiter);
                    }
                }
            }
        }
    }

    const TChar *c_str()
    {
        return m_list.c_str();
    }

private:
    TChar m_delimiter;
    std::basic_string<TChar> m_list;
    std::set< std::basic_string<TChar> > m_added;
};

#endif // __CLRHOST_TPALIST_BUILDER_H__
