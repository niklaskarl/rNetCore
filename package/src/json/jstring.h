#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__

#include "json.h"
#include <cstddef>

class JStringImpl : public Json::JString
{
    public:
    JStringImpl(const char* value, std::size_t count);

    // Overridden implementations of JToken
    bool IsValid();
    bool IsString(std::string &value);

    Json::JString *AsString();

    // Custom JString implementations
    std::size_t GetByteLength();

    Json::JStringIterator Begin();
    Json::JStringIterator End();

    private:
    const char *m_value;
    std::size_t m_count;
};

#endif // __JSON_STRING_H__
