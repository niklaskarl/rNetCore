#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__

#include "json.h"
#include <cstddef>
#include <vector>

class JArrayImpl : public Json::JArray
{
    public:
    JArrayImpl();

    // Overridden implementations of JToken
    bool IsValid();

    Json::JArray *AsArray();
    Json::JToken *GetArrayItem(std::size_t index);

    void AddValue(Json::JToken *const value);

    private:
    std::vector<Json::JToken *> m_values;
};

#endif // __JSON_ARRAY_H__
