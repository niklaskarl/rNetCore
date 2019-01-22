#ifndef __JSON_VALUE_H__
#define __JSON_VALUE_H__

#include "json.h"

Json::JToken *GetJsonInvalidValue();
Json::JToken *GetJsonNullValue();
Json::JToken *GetJsonTrueValue();
Json::JToken *GetJsonFalseValue();

Json::JToken *ConstructJsonNumber(double value);

#endif // __JSON_VALUE_H__
