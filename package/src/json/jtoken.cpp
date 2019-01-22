#include "json.h"
#include "jvalue.h"

bool Json::JToken::IsValid()
{
    return false;
}

bool Json::JToken::IsNull()
{
    return false;
}

bool Json::JToken::IsBoolean(bool *value)
{
    if (value)
    {
        *value = false;
    }

    return false;
}

bool Json::JToken::IsNumber(double *value)
{
    if (value)
    {
        *value = 0.0;
    }

    return false;
}

bool Json::JToken::IsString(std::string &value)
{
    value.clear();
    return false;
}

Json::JString *Json::JToken::AsString()
{
    return NULL;
}

Json::JObject *Json::JToken::AsObject()
{
    return NULL;
}

Json::JArray *Json::JToken::AsArray()
{
    return NULL;
}

Json::JToken *Json::JToken::GetArrayItem(std::size_t index)
{
    return GetJsonInvalidValue();
}

Json::JToken *Json::JToken::GetObjectValue(const char *key)
{
    return GetJsonInvalidValue();
}
