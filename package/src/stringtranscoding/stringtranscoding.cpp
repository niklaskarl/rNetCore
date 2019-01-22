#include "stringtranscoding.h"

void utf8_to_utf16(const utf8char_t *src, utf16string &dest)
{
    CREATE_ITER_U8_U16(iter, const utf8char_t *, src);
    utf16char_t c;
    for (c = *iter; c != '\0'; c = *++iter)
    {
        size_t length = dest.length();
        if (length == dest.capacity())
        {
            dest.reserve(length + 10);
        }

        dest.push_back(c);
    }
}

utf16string utf8_to_utf16(const utf8char_t *src)
{
    utf16string dest;
    utf8_to_utf16(src, dest);
    return dest;
}

void utf8_to_utf16(const utf8char_t *src, const size_t &n, utf16string &dest)
{
    CREATE_ITER_U8_U16(begin, const utf8char_t *, src);
    CREATE_ITER_U8_U16(end, const utf8char_t *, src + n);

    dest.append(begin, end);
}

utf16string utf8_to_utf16(const utf8char_t *src, const size_t &n)
{
    utf16string dest;
    utf8_to_utf16(src, n, dest);
    return dest;
}

void utf16_to_utf8(const utf16char_t *src, utf8string &dest)
{
    CREATE_ITER_U16_U8(iter, const utf16char_t *, src);
    utf8char_t c;
    for (c = *iter; c != '\0'; c = *++iter)
    {
        size_t length = dest.length();
        if (length == dest.capacity())
        {
            dest.reserve(length + 10);
        }

        dest.push_back(c);
    }
}

utf8string utf16_to_utf8(const utf16char_t *src)
{
    utf8string dest;
    utf16_to_utf8(src, dest);
    return dest;
}

void utf16_to_utf8(const utf16char_t *src, const size_t &n, utf8string &dest)
{
    CREATE_ITER_U16_U8(begin, const utf16char_t *, src);
    CREATE_ITER_U16_U8(end, const utf16char_t *, src + n);

    dest.append(begin, end);
}

utf8string utf16_to_utf8(const utf16char_t *src, const size_t &n)
{
    utf8string dest;
    utf16_to_utf8(src, n, dest);
    return dest;
}
