#ifndef __JSON_H__
#define __JSON_H__

#include <inttypes.h>
#include <cstddef>
#include <string>

namespace Json
{
    class JToken;
    class JString;
    class JStringIterator;
    class JObject;
    class JArray;

    struct JProperty
    {
        Json::JString *key;
        Json::JToken *value;
    };

    class JToken
    {
    public:
        virtual bool IsValid();

        virtual bool IsNull();
        virtual bool IsBoolean(bool *value);
        virtual bool IsNumber(double *value);
        virtual bool IsString(std::string &value);

        virtual JString *AsString();
        virtual JObject *AsObject();
        virtual JArray *AsArray();

        virtual JToken *GetArrayItem(std::size_t index);
        virtual JToken *GetObjectValue(const char *key);
    };

    class JString : public JToken
    {
    public:
        virtual std::size_t GetByteLength() = 0;

        virtual JStringIterator Begin() = 0;
        virtual JStringIterator End() = 0;
    };

    class JObject : public JToken
    {
    public:
    };

    class JArray : public JToken
    {
    public:
    };

    class JStringIterator : public std::iterator<std::input_iterator_tag, char>
    {
    public:
        JStringIterator(const char *str, size_t len, size_t pos);
        JStringIterator(const JStringIterator& dec);

        JStringIterator& operator++();
        JStringIterator operator++(int);

        bool operator==(const JStringIterator& rhs) const;
        bool operator!=(const JStringIterator& rhs) const;
        char operator*();

    private:
        const char *m_str;
        size_t m_len;
        size_t m_pos;
        uint8_t m_byte;
    };

    extern "C" bool ParseJson(const char* json, JToken **root);
}

#endif // __JSON_H__
