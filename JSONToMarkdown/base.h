#pragma once
#ifndef GNOIX_INL_BASE_H_
#define GNOIX_INL_BASE_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "Encoding.h"
#include "json.hpp"
using json = nlohmann::json;
using json_value_t = nlohmann::detail::value_t;
#ifdef _WIN32
const char dir_separators[] = "/\\";
#else
const char dir_separators[] = "/";
#endif

#endif // !GNOIX_INL_BASE_H_
