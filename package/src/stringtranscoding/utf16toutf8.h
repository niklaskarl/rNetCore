#ifndef __UTF16_TO_UTF8_TRANSCODING_H__
#define __UTF16_TO_UTF8_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

class Utf16ToUtf8Iterator : public std::iterator<std::input_iterator_tag, utf8char_t>
{
public:
    Utf16ToUtf8Iterator(const utf16char_t *str = NULL, ptrdiff_t len = -1, ptrdiff_t pos = 0);
    Utf16ToUtf8Iterator(const Utf16ToUtf8Iterator &iter);

    operator bool() const;

    bool operator==(const Utf16ToUtf8Iterator &rhs) const;
    bool operator!=(const Utf16ToUtf8Iterator &rhs) const;

    Utf16ToUtf8Iterator &operator++();
    Utf16ToUtf8Iterator operator++(int);

    utf8char_t operator*() const;

private:
    const utf16char_t *m_str;
    ptrdiff_t m_len;
    ptrdiff_t m_pos;
    uint8_t m_subPos;
};

#endif // __UTF16_TO_UTF8_TRANSCODING_H__
