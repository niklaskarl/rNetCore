#ifndef __STRING_TRANSCODING_TYPES_H__
#define __STRING_TRANSCODING_TYPES_H__

#include <inttypes.h>
#include <string>

#if __cplusplus > 199711L || _MSC_VER >= 1900 // C++11

typedef char utf8char_t;
typedef char16_t utf16char_t;
typedef char32_t utf32char_t;

#define utf8string std::string
#define utf16string std::basic_string< char16_t >
#define utf32string std::basic_string< char32_t >

#define _U8 u8""
#define _U16 u""
#define _U32 U""

#else // No C++11

typedef char utf8char_t;
typedef wchar_t utf16char_t;
typedef uint32_t utf32char_t;

#define utf8string std::string
#define utf16string std::basic_string< wchar_t >
#define utf32string std::basic_string< uint32_t >

#define _U8
#define _U16 L""

#endif // C++11


#endif // __STRING_TRANSCODING_TYPES_H__
