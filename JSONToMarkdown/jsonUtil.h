#pragma once
#ifndef GNOIX_INL_JSON_UTIL_H
#define GNOIX_INL_JSON_UTIL_H
#include "base.h"
class jsonUtil
{
public:
	static std::string transferStr(const json_value_t &type);
};

#endif // !GNOIX_INL_JSON_UTIL_H