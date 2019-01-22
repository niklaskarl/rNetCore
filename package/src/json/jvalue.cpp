#include "jvalue.h"

class JNull : public Json::JToken
{
    public:
    bool IsValid()
    {
        return true;
    }

    bool IsNull()
    {
        return true;
    }
};

class JBoolean : public Json::JToken
{
    public:
    JBoolean(bool value)
    {
        m_value = value;
    }

    bool IsValid()
    {
        return true;
    }

    bool IsBoolean(bool *value)
    {
        if (value)
        {
            *value = m_value;
        }

        return true;
    }

    private:
    bool m_value;
};

class JNumber : public Json::JToken
{
    public:
    JNumber(double value)
    {
        m_value = value;
    }

    bool IsValid()
    {
        return true;
    }

    bool IsNumber(double *value)
    {
        if (value)
        {
            *value = m_value;
        }

        return true;
    }

    private:
    double m_value;
};

Json::JToken g_invalid = Json::JToken();
JNull g_null = JNull();
JBoolean g_false = JBoolean(false);
JBoolean g_true = JBoolean(true);

Json::JToken *GetJsonInvalidValue()
{
    return &g_invalid;
}

Json::JToken *GetJsonNullValue()
{
    return &g_null;
}

Json::JToken *GetJsonFalseValue()
{
    return &g_false;
}

Json::JToken *GetJsonTrueValue()
{
    return &g_true;
}

Json::JToken *ConstructJsonNumber(double value)
{
    return new JNumber(value);
}
