#pragma once
#ifndef GNOIX_INL_UTIL_H_
#define GNOIX_INL_UTIL_H_
#include <algorithm>
#include "base.h"
namespace gnoixinl {

	std::string readFile(const char* filePath, std::ios_base::open_mode openMode = std::ios_base::in);

	template <typename T>
	int writeFile(const char* filePath, T t, std::ios_base::open_mode openMode = std::ios_base::out);

    std::string join(const std::string& base, const std::string& path);

    void glob(const std::string& pattern, std::vector<std::string>& result, bool recursive = false);

    static inline bool isPathSeparator(char c);

    std::string getUTFFileName(const std::string& utf8_filePath, bool withExtension = true);

    std::string getAnsiFileName(const std::string& ansi_filePath, bool withExtension = true);
}
#endif // !GNOIX_INL_UTIL_H_

