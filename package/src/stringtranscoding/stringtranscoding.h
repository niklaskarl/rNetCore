#ifndef __STRING_TRANSCODING_H__
#define __STRING_TRANSCODING_H__

#include <iterator>
#include <string>

#include "stringtypes.h"

#include "utf8toutf32.h"
#include "utf32toutf8.h"
#include "utf16toutf32.h"
#include "utf32toutf16.h"

#define INITIALIZE_ITER_U8_U32(src_type, src) Utf8ToUtf32Iterator< src_type >(src)
#define CREATE_ITER_U8_U32(name, src_type, src) Utf8ToUtf32Iterator< src_type > name = INITIALIZE_ITER_U8_U32(src_type, src)

#define INITIALIZE_ITER_U32_U8(src_type, src) Utf32ToUtf8Iterator< src_type >(src)
#define CREATE_ITER_U32_U8(name, src_type, src) Utf32ToUtf8Iterator< src_type > name = INITIALIZE_ITER_U32_U8(src_type, src)

#define INITIALIZE_ITER_U16_U32(src_type, src) Utf16ToUtf32Iterator< src_type >(src)
#define CREATE_ITER_U16_U32(name, src_type, src) Utf16ToUtf32Iterator< src_type > name = INITIALIZE_ITER_U16_U32(src_type, src)

#define INITIALIZE_ITER_U32_U16(src_type, src) Utf32ToUtf16Iterator< src_type >(src)
#define CREATE_ITER_U32_U16(name, src_type, src) Utf32ToUtf16Iterator< src_type > name = INITIALIZE_ITER_U32_U16(src_type, src)

#define INITIALIZE_ITER_U8_U16(src_type, src) Utf32ToUtf16Iterator< Utf8ToUtf32Iterator< src_type > >(Utf8ToUtf32Iterator< src_type > (src))
#define CREATE_ITER_U8_U16(name, src_type, src) Utf32ToUtf16Iterator< Utf8ToUtf32Iterator< src_type > > name = INITIALIZE_ITER_U8_U16(src_type, src)

#define CREATE_ITER_U16_U8(name, src_type, src) Utf32ToUtf8Iterator< Utf16ToUtf32Iterator< src_type > > name = INITIALIZE_ITER_U16_U32(src_type, src)
#define INITIALIZE_ITER_U16_U8(src_type, src) Utf32ToUtf8Iterator< Utf16ToUtf32Iterator< src_type > >(Utf16ToUtf32Iterator< src_type > (src))

void utf8_to_utf16(const utf8char_t *src, utf16string &dest);
utf16string utf8_to_utf16(const utf8char_t *src);

void utf8_to_utf16(const utf8char_t *src, const size_t &n, utf16string &dest);
utf16string utf8_to_utf16(const utf8char_t *src, const size_t &n);

template < class T >
void utf8_to_utf16(const T &src, utf16string &dest)
{
    utf8_to_utf16(src.c_str(), src.size(), dest);
}
template < class T >
utf16string utf8_to_utf16(const T &src)
{
    return utf8_to_utf16(src.c_str(), src.size());
}

void utf16_to_utf8(const utf16char_t *src, utf8string &dest);
utf8string utf16_to_utf8(const utf16char_t *src);

void utf16_to_utf8(const utf16char_t *src, const size_t &n, utf8string &dest);
utf8string utf16_to_utf8(const utf16char_t *src, const size_t &n);

template < class T >
void utf16_to_utf8(const T &src, utf8string &dest)
{
    utf16_to_utf8(src.c_str(), src.size(), dest);
}
template < class T >
utf8string utf16_to_utf8(const T &src)
{
    return utf16_to_utf8(src.c_str(), src.size());
}

#endif // __STRING_TRANSCODING_H__
