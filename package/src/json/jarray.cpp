#include "jarray.h"
#include "jvalue.h"

JArrayImpl::JArrayImpl()
{
    m_values = std::vector<Json::JToken *>();
}

bool JArrayImpl::IsValid()
{
    return true;
}

Json::JArray *JArrayImpl::AsArray()
{
    return this;
}

Json::JToken *JArrayImpl::GetArrayItem(std::size_t index)
{
    if (index >= 0 && index < m_values.size())
    {
        return m_values[index];
    }

    return GetJsonInvalidValue();
}

void JArrayImpl::AddValue(Json::JToken *const value)
{
    m_values.push_back(value);
}
