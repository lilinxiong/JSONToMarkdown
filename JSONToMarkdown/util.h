#pragma once
#ifndef UTIL_H_
#define UTIL_H_
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "base.h"
namespace gnoixinl {

    std::string readFile(const char* filePath);

    void writeFile(const char* filePath);

    std::string join(const std::string& base, const std::string& path);

    void glob(std::string pattern, std::vector<std::string>& result, bool recursive = false);

    static bool isDir(const std::string& path, DIR* dir);

    static inline bool isPathSeparator(char c);

    std::string getUTFFileName(const std::string& utf8_filePath, bool withExtension = true);

    std::string getAnsiFileName(const std::string& ansi_filePath, bool withExtension = true);
}
#endif // !UTIL_H_

