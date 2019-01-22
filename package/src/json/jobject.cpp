#include "jobject.h"
#include "jvalue.h"

template<typename Iter1, typename Iter2>
int compare(Iter1 iter1, Iter2 iter2)
{
    while (*iter1 || *iter2)
    {
        if (*iter1 != *iter2)
        {
            return *iter2 - *iter1;
        }

        ++iter1;
        ++iter2;
    }

    return 0;
}

JObjectImpl::JObjectImpl()
{
    m_values = std::vector<Json::JProperty>();
}

bool JObjectImpl::IsValid()
{
    return true;
}

Json::JObject *JObjectImpl::AsObject()
{
    return this;
}

Json::JToken *JObjectImpl::GetObjectValue(const char *key)
{
    for (size_t i = 0; i < m_values.size(); i++) {
        if (compare(m_values[i].key->Begin(), key) == 0)
        {
            return m_values[i].value;
        }
    }

    return GetJsonInvalidValue();
}

bool JObjectImpl::AddValue(Json::JString *key, Json::JToken *value)
{
    for (size_t i = 0; i < m_values.size(); i++) {
        if (compare(m_values[i].key->Begin(), key->Begin()) == 0)
        {
            return false;
        }
    }

    Json::JProperty property = { 0 };
    property.key = key;
    property.value = value;

    m_values.push_back(property);
    return true;
}
