#include "jstring.h"

#include <string.h>

JStringImpl::JStringImpl(const char* value, std::size_t count)
{
    m_value = value;
    m_count = count;
}

bool JStringImpl::IsValid()
{
    return true;
}

bool JStringImpl::IsString(std::string &value)
{
    value.clear();
    value.append(Begin(), End());

    return true;
}

Json::JString *JStringImpl::AsString()
{
    return this;
}

std::size_t JStringImpl::GetByteLength()
{
    return m_count;
}

Json::JStringIterator JStringImpl::Begin()
{
    return Json::JStringIterator(m_value, m_count, 0);
}

Json::JStringIterator JStringImpl::End()
{
    return Json::JStringIterator(m_value, m_count, m_count);
}
