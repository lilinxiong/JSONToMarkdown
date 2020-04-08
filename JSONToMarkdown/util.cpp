#include "util.h"
#include <assert.h>
#include <sstream>
#include <codecvt>
#if defined _WIN32
# include <windows.h>
struct dirent
{
	const char* d_name;
};

struct DIR
{
	WIN32_FIND_DATAA data;
	HANDLE handle;
	dirent ent;
};

DIR* opendir(const char* path)
{
	DIR* dir = new DIR;
	dir->ent.d_name = 0;
	dir->handle = ::FindFirstFileExA((std::string(path) + "\\*").c_str(),
		FindExInfoStandard, &dir->data, FindExSearchNameMatch, NULL, 0);
	if (dir->handle == INVALID_HANDLE_VALUE) {
		/*closedir will do all cleanup*/
		delete dir;
		return 0;
	}
	return dir;
}

dirent* readdir(DIR* dir)
{
	if (dir->ent.d_name != 0) {
		if (::FindNextFileA(dir->handle, &dir->data) != TRUE)
			return 0;
	}
	dir->ent.d_name = dir->data.cFileName;
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

namespace gnoixinl {
	static bool isDir(const std::string& path, DIR* dir);
	static bool wildcmp(const char* string, const char* wild)
	{
		const char* cp = 0, *mp = 0;

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
			} else if ((*wild == *string) || (*wild == '?')) {
				wild++;
				string++;
			} else {
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
		bool recursive, bool includeDirectories, const std::string& pathPrefix)
	{
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

					if (isDir(path, dir)) {
						if (recursive)
							glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
						if (!includeDirectories)
							continue;
					}

					if (wildchart.empty() || wildcmp(name, wildchart.c_str()))
						result.push_back(entry);
				}
			} catch (...) {
				closedir(dir);
				throw;
			}
			closedir(dir);
		} else {
			//LOGE("could not open directory: %s", directory.c_str());
		}
	}

	std::string readFile(const char* filePath, std::ios_base::open_mode openMode)
	{
		std::wifstream wif(filePath);
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
		wss << wif.rdbuf();
		wif.clear();
		wif.close();
		return wcs_to_utf8(wss.str());
	}

	template <typename T>
	int writeFile(const char* filePath, T t, std::ios_base::open_mode openMode)
	{
		std::ofstream ofs;
		ofs.open(filePath, openMode);
		if (!ofs.is_open()) {
			std::cout << "open " << filePath << " faild!" << std::endl;
			return -1;
		}
		ofs << t << std::endl;
		ofs.clear();
		ofs.close();
		return 0;
	}

	std::string join(const std::string& base, const std::string& path)
	{
		if (base.empty())
			return path;
		if (path.empty())
			return base;

		bool baseSep = isPathSeparator(base[base.size() - 1]);
		bool pathSep = isPathSeparator(path[0]);
		std::string result;
		if (baseSep && pathSep) {
			result = base + path.substr(1);
		} else if (!baseSep && !pathSep) {
			result = base + dir_separators + path;
		} else {
			result = base + path;
		}
		return result;
	}

	void glob(const std::string& pattern, std::vector<std::string>& result, bool recursive)
	{
		result.clear();
		std::string path, wildchart;

		if (isDir(pattern, 0)) {
			if (strchr(dir_separators, pattern[pattern.size() - 1]) != 0) {
				path = pattern.substr(0, pattern.size() - 1);
			} else {
				path = pattern;
			}
		} else {
			size_t pos = pattern.find_last_of(dir_separators);
			if (pos == std::string::npos) {
				wildchart = pattern;
				path = ".";
			} else {
				path = pattern.substr(0, pos);
				wildchart = pattern.substr(pos + 1);
			}
		}
		glob_rec(path, wildchart, result, recursive, false, path);
		std::sort(result.begin(), result.end());
	}

	static bool isDir(const std::string& path, DIR* dir)
	{
#if defined _WIN32
		DWORD attributes;
		BOOL status = TRUE;
		if (dir)
			attributes = dir->data.dwFileAttributes;
		else {
			WIN32_FILE_ATTRIBUTE_DATA all_attrs;
			status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
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

	static inline bool isPathSeparator(char c)
	{
		return c == '/' || c == '\\';
	}

	std::string getUTFFileName(const std::string& utf8_filePath, bool withExtension)
	{
		std::size_t dotPos = utf8_filePath.rfind('.');
		std::size_t sepPos = utf8_filePath.rfind(dir_separators);
		if (sepPos != std::string::npos) {
			return utf8_filePath.substr(sepPos + 1, utf8_filePath.size() - (withExtension || dotPos != std::string::npos ? 1 : dotPos));
		}
		return "";
	}

	std::string getAnsiFileName(const std::string& ansi_filePath, bool withExtension)
	{
		std::wstring wcs_filePath = ansi_to_wcs(ansi_filePath);
		std::string utf8_filePath = wcs_to_utf8(wcs_filePath);
		return getUTFFileName(utf8_filePath.c_str(), withExtension);
	}
}
