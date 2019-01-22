#ifndef __UTF8_TO_UTF16_TRANSCODING_H__
#define __UTF8_TO_UTF16_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

class Utf8ToUtf16Iterator : public std::iterator<std::input_iterator_tag, utf16char_t>
{
public:
    Utf8ToUtf16Iterator(const utf8char_t *str = NULL, ptrdiff_t len = -1, ptrdiff_t pos = 0);
    Utf8ToUtf16Iterator(const Utf8ToUtf16Iterator &iter);

    operator bool() const;

    bool operator==(const Utf8ToUtf16Iterator &rhs) const;
    bool operator!=(const Utf8ToUtf16Iterator &rhs) const;

    Utf8ToUtf16Iterator &operator++();
    Utf8ToUtf16Iterator operator++(int);

    utf16char_t operator*() const;

private:
    const utf8char_t *m_str;
    ptrdiff_t m_len;
    ptrdiff_t m_pos;
    uint8_t m_subPos;
};

#endif // __UTF8_TO_UTF16_TRANSCODING_H__
