#include "util.h"

static bool wildcmp(const char* string, const char* wild)
{
    const char* cp = 0, * mp = 0;

    while ((*string) && (*wild != '*')) {
        if ((*wild != *string) && (*wild != '?')) {
            return false;
        }
        wild++;
        string++;
    }

    while (*string) {
        if (*wild == '*') {
            if (!*++wild) {
                return true;
            }
            mp = wild;
            cp = string + 1;
        }
        else if ((*wild == *string) || (*wild == '?')) {
            wild++;
            string++;
        }
        else {
            wild = mp;
            string = cp++;
        }
    }

    while (*wild == '*') {
        wild++;
    }
    return *wild == 0;
}

static void glob_rec(const std::string& directory, const std::string& wildchart, std::vector<std::string>& result,
    bool recursive, bool includeDirectories, const std::string& pathPrefix) {
    DIR* dir;

    if ((dir = opendir(directory.c_str())) != 0) {
        /* find all the files and directories within directory */
        try {
            struct dirent* ent;
            while ((ent = readdir(dir)) != 0) {
                const char* name = ent->d_name;
                if ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    continue;

                std::string path = gnoixinl::join(directory, name);
                std::string entry = gnoixinl::join(pathPrefix, name);

                if (gnoixinl::isDir(path, dir)) {
                    if (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    if (!includeDirectories)
                        continue;
                }

                if (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        catch (...) {
            closedir(dir);
            throw;
        }
        closedir(dir);
    }
    else {
        //LOGE("could not open directory: %s", directory.c_str());
    }
}

std::string gnoixinl::readFile(const char* filePath) {
    return "";
}

void gnoixinl::writeFile(const char* filePath) {
    return;
}

std::string gnoixinl::join(const std::string& base, const std::string& path) {
    if (base.empty())
        return path;
    if (path.empty())
        return base;

    bool baseSep = isPathSeparator(base[base.size() - 1]);
    bool pathSep = isPathSeparator(path[0]);
    std::string result;
    if (baseSep && pathSep) {
        result = base + path.substr(1);
    }
    else if (!baseSep && !pathSep) {
        result = base + dir_separator + path;
    }
    else {
        result = base + path;
    }
    return result;
}

void gnoixinl::glob(std::string pattern, std::vector<std::string>& result, bool recursive) {
    result.clear();
    std::string path, wildchart;

    if (isDir(pattern, 0)) {
        if (strchr(dir_separator, pattern[pattern.size() - 1]) != 0) {
            path = pattern.substr(0, pattern.size() - 1);
        }
        else {
            path = pattern;
        }
    }
    else {
        size_t pos = pattern.find_last_of(dir_separator);
        if (pos == std::string::npos) {
            wildchart = pattern;
            path = ".";
        }
        else {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }
    glob_rec(path, wildchart, result, recursive, false, path);
    std::sort(result.begin(), result.end());
}

static bool gnoixinl::isDir(const std::string& path, DIR* dir) {
#if defined _WIN32 || defined WINCE
    DWORD attributes;
    BOOL status = TRUE;
    if (dir)
        attributes = dir->data.dwFileAttributes;
    else
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#else
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    return status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
    EVA_UNUSED(dir);
    struct stat stat_buf;
    if (0 != stat(path.c_str(), &stat_buf))
        return false;
    int is_dir = S_ISDIR(stat_buf.st_mode);
    return is_dir != 0;
#endif
}

static inline bool gnoixinl::isPathSeparator(char c) {
    return c == '/' || c == '\\';
}

std::string gnoixinl::getUTFFileName(const std::string& utf8_filePath, bool withExtension) {
    std::size_t dotPos = utf8_filePath.rfind('.');
    std::size_t sepPos = utf8_filePath.rfind(dir_separator);
    if (sepPos != std::string::npos) {
        return utf8_filePath.substr(sepPos + 1, utf8_filePath.size() - (withExtension || dotPos != std::string::npos ? 1 : dotPos));
    }
    return "";
}

std::string gnoixinl::getAnsiFileName(const std::string& ansi_filePath, bool withExtension) {
    std::wstring wcs_filePath = ansi_to_wcs(ansi_filePath);
    std::string utf8_filePath = wcs_to_utf8(wcs_filePath);
    return getUTFFileName(utf8_filePath.c_str(), withExtension);
}
