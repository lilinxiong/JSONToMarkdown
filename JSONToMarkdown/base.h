#pragma once
#ifndef BASE_H_
#define BASE_H_
#include "Encoding.h"

#ifdef _WIN32
const char dir_separator[] = "/\\";
#else
const char dir_separator[] = "/";
#endif

#if defined _WIN32 || defined WINCE
# include <windows.h>
struct dirent
{
    const char* d_name;
};

struct DIR
{
#if defined(WINRT) || defined(_WIN32_WCE)
    WIN32_FIND_DATAW data;
#else
    WIN32_FIND_DATAA data;
#endif
    HANDLE handle;
    dirent ent;
#ifdef WINRT
    DIR() { }
    ~DIR()
    {
        if (ent.d_name)
            delete[] ent.d_name;
    }
#endif
};

DIR* opendir(const char* path)
{
    DIR* dir = new DIR;
    dir->ent.d_name = 0;
#if defined(WINRT) || defined(_WIN32_WCE)
    cv::String full_path = cv::String(path) + "\\*";
    wchar_t wfull_path[MAX_PATH];
    size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
        &dir->data, FindExSearchNameMatch, NULL, 0);
#else
    dir->handle = ::FindFirstFileExA((std::string(path) + "\\*").c_str(),
        FindExInfoStandard, &dir->data, FindExSearchNameMatch, NULL, 0);
#endif
    if (dir->handle == INVALID_HANDLE_VALUE)
    {
        /*closedir will do all cleanup*/
        delete dir;
        return 0;
    }
    return dir;
}

dirent* readdir(DIR* dir)
{
#if defined(WINRT) || defined(_WIN32_WCE)
    if (dir->ent.d_name != 0)
    {
        if (::FindNextFileW(dir->handle, &dir->data) != TRUE)
            return 0;
    }
    size_t asize = wcstombs(NULL, dir->data.cFileName, 0);
    CV_Assert((asize != 0) && (asize != (size_t)-1));
    char* aname = new char[asize + 1];
    aname[asize] = 0;
    wcstombs(aname, dir->data.cFileName, asize);
    dir->ent.d_name = aname;
#else
    if (dir->ent.d_name != 0)
    {
        if (::FindNextFileA(dir->handle, &dir->data) != TRUE)
            return 0;
    }
    dir->ent.d_name = dir->data.cFileName;
#endif
    return &dir->ent;
}

void closedir(DIR* dir)
{
    ::FindClose(dir->handle);
    delete dir;
}

#else
# include <dirent.h>
# include <sys/stat.h>
#endif

#endif // !BASE_H_
