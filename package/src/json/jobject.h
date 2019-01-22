#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include "json.h"
#include <cstddef>
#include <vector>

class JObjectImpl : public Json::JObject
{
    public:
    JObjectImpl();

    // Overridden implementations of JToken
    bool IsValid();

    Json::JObject *AsObject();
    Json::JToken *GetObjectValue(const char *key);

    bool AddValue(Json::JString *key, Json::JToken *value);

    private:
    std::vector<Json::JProperty> m_values;
};

#endif // __JSON_OBJECT_H__
