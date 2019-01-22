#include "json.h"
#include "jvalue.h"
#include "jstring.h"
#include "jarray.h"
#include "jobject.h"

#include <stdlib.h>

bool IsHex(char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';

    puts(buf);

    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

class JsonParser
{
    public:
    JsonParser(const char *json)
    {
        m_json = json;
        m_current = m_json;
    };

    bool Parse(Json::JToken **token)
    {
        bool result = ParseToken(token);

        if (result)
        {
            SkipWhitespace();
            if (*m_current != '\0')
            {
                // ERROR expected end of file
                puts("ERROR expected end of file\n");
                // TODO cleanup token
                *token = NULL;
                result = false;
            }
        }

        return result;
    }

    private:
    bool ParseToken(Json::JToken **token)
    {
        SkipWhitespace();

        bool result = false;
        switch (*m_current)
        {
            case '{':
            result = ParseObject((Json::JObject **)token);
            break;
            case '[':
            result = ParseArray((Json::JArray **)token);
            break;
            case '\"':
            result = ParseString((Json::JString **)token);
            break;
            case 'n':
            result = ParseNull(token);
            break;
            case 'f':
            result = ParseFalse(token);
            break;
            case 't':
            result = ParseTrue(token);
            break;
            default:
            result = ParseNumber(token);
            break;
        }

        return result;
    };

    bool ParseObject(Json::JObject **result)
    {
        if (*m_current != '{')
        {
            // ERROR expected object start indicator
            puts("ERROR expected object start indicator\n");
            return false;
        }

        m_current++;

        JObjectImpl *object = new JObjectImpl();

        SkipWhitespace();

        bool done = *m_current == '}';
        while (!done)
        {
            Json::JString *key = NULL;
            Json::JToken *value = NULL;
            if (!ParseString(&key))
            {
                // ERROR parse key failed
                puts("ERROR parse key failed\n");
                // TODO cleanup object
                return false;
            }

            SkipWhitespace();
            if (*m_current != ':')
            {
                // ERROR property assignment operator expected
                puts("ERROR property assignment operator expected\n");
                // TODO cleanup object
                return false;
            }

            m_current++;
            SkipWhitespace();
            if (!ParseToken(&value))
            {
                // ERROR parse value failed
                puts("ERROR parse value failed\n");
                // TODO cleanup object
                return false;
            }

            if (!object->AddValue(key, value))
            {
                // ERROR duplicate key
                puts("ERROR duplicate key\n");
                // TODO cleanup object
                return false;
            }

            SkipWhitespace();
            done = *m_current != ',';
            if (!done)
            {
                m_current++;
                SkipWhitespace();
            }
        }

        if (*m_current != '}')
        {
            // ERROR expected object end indicator
            puts("ERROR expected object end indicator\n");
            // TODO cleanup object
            return false;
        }

        m_current++;
        *result = object;
        return true;
    }

    bool ParseArray(Json::JArray **result)
    {
        if (*m_current != '[')
        {
            // ERROR expected array start indicator
            puts("ERROR expected array start indicator\n");
            return false;
        }

        m_current++;

        JArrayImpl *array = new JArrayImpl();

        SkipWhitespace();

        bool done = *m_current == ']';
        while (!done)
        {
            Json::JToken *item = NULL;
            if (!ParseToken(&item))
            {
                // ERROR parse item failed
                puts("ERROR parse item failed\n");
                // TODO cleanup array
                return false;
            }

            array->AddValue(item);
            SkipWhitespace();
            done = *m_current != ',';
            if (!done)
            {
                m_current++;
                SkipWhitespace();
            }
        }

        if (*m_current != ']')
        {
            // ERROR expected array end indicator
            puts("ERROR expected array end indicator\n");
            // TODO cleanup array
            return false;
        }

        m_current++;
        *result = array;
        return true;
    }

    bool ParseNull(Json::JToken **result)
    {
        if (*m_current == 'n' &&
            *(++m_current) == 'u' &&
            *(++m_current) == 'l' &&
            *(++m_current) == 'l')
        {
            m_current++;
            *result = GetJsonNullValue();
            return true;
        }

        // ERROR expected value
        puts("ERROR expected value\n");
        return false;
    }

    bool ParseFalse(Json::JToken **result)
    {
        if (*m_current == 'f' &&
            *(++m_current) == 'a' &&
            *(++m_current) == 'l' &&
            *(++m_current) == 's' &&
            *(++m_current) == 'e')
        {
            m_current++;
            *result = GetJsonFalseValue();
            return true;
        }

        // ERROR expected value
        puts("ERROR expected value\n");
        return false;
    }

    bool ParseTrue(Json::JToken **result)
    {
        if (*(m_current) == 't' &&
            *(++m_current) == 'r' &&
            *(++m_current) == 'u' &&
            *(++m_current) == 'e')
        {
            m_current++;
            *result = GetJsonTrueValue();
            return true;
        }

        // ERROR expected value
        puts("ERROR expected value\n");
        return false;
    }

    bool ParseNumber(Json::JToken **result)
    {
        const char *start = m_current;

        if (*m_current == '-')
        {
            m_current++;
        }

        if (*m_current < '0' || *m_current > '9')
        {
            // ERROR expected digit
            puts("ERROR expected digit\n");
            return false;
        }

        m_current++;
        while (*m_current >= '0' && *m_current <= '9')
        {
            m_current++;
        }

        if (*m_current == '.')
        {
            m_current++;
            while (*m_current >= '0' && *m_current <= '9')
            {
                m_current++;
            }
        }

        if (*m_current == 'e' || *m_current == 'E')
        {
            m_current++;
            if (*m_current == '+' || *m_current == '-')
            {
                m_current++;
            }

            if (*m_current < '0' || *m_current > '9')
            {
                // ERROR expected digit
                puts("ERROR expected digit\n");
                return false;
            }

            m_current++;
            while (*m_current >= '0' && *m_current <= '9')
            {
                m_current++;
            }
        }

        std::string buffer;
        buffer.assign(start, m_current - start);
        
        puts(buffer.c_str());
        double value = strtod(buffer.c_str(), NULL);

        *result = ConstructJsonNumber(value);
        return true;
    }

    bool ParseString(Json::JString **result)
    {
        if (*m_current != '\"')
        {
            // ERROR expected string start indicator
            puts("ERROR expected string start indicator\n");
            return false;
        }

        m_current++;

        const char* start = m_current;
        while (*m_current != '\"' && *m_current != '\0')
        {
            /*if (*m_current < 0x0020)
            {
                // ERROR invalid character found
                puts("ERROR invalid character found\n");
                return false;
            }*/

            if (*m_current == '\\')
            {
                m_current++;
                switch (*m_current)
                {
                    case '\"':
                    case '\\':
                    case '/':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                        break;
                    case 'u':
                        if (!IsHex(*(++m_current)) ||
                            !IsHex(*(++m_current)) ||
                            !IsHex(*(++m_current)) ||
                            !IsHex(*(++m_current)))
                        {
                            // ERROR expexted four hex values
                            puts("ERROR expexted four hex values\n");
                            return false;
                        }

                        break;
                    default:
                        // ERROR invalid escape sequence found
                        puts("ERROR invalid escape sequence found\n");
                        return false;
                }
            }

            m_current++;
        }

        if (*m_current == '\0')
        {
            // ERROR expected string end indicator
            puts("ERROR expected string end indicator\n");
            return false;
        }

        std::size_t length = m_current - start;
        *result = (Json::JString *)new JStringImpl(start, length);
        m_current++;
        return true;
    }

    void SkipWhitespace()
    {
        // TODO add more whitespace characters
        while (
            *m_current == ' ' ||
            *m_current == '\t' ||
            *m_current == '\r' ||
            *m_current == '\n')
        {
            m_current++;
        }
    }

    private:
    const char *m_json;
    const char *m_current;
};

bool Json::ParseJson(const char *json, Json::JToken **root)
{
    JsonParser parser(json);
    return parser.Parse(root);
}
