#include <stdlib.h>
#include <stdint.h>
#include "Encoding.h"

#ifdef _WIN32
#include <Windows.h>
#endif

void convert_content(std::wstring & wstr)
{
	for (size_t i = 0; i < wstr.size(); i++)
	{
		if (wstr[i] > 0xFF00) {
			wstr[i] -= (0xFF00 - 0x20);
		}
	}
}

void convert_content(wchar_t & w)
{
	if (w > 0xFF00) {
		w -= (0xFF00 - 0x20);
	}
}

std::wstring ansi_to_wcs(const std::string &ansi)
{
#ifdef _WIN32
    int n = MultiByteToWideChar(CP_ACP, 0, ansi.data(), -1, NULL, 0);
    wchar_t *s = new wchar_t[n + 1];
    MultiByteToWideChar(CP_ACP, 0, ansi.data(), -1, s, n + 1);
    s[n] = '\0';
    std::wstring dst(s);
#else
    char *locale = setlocale(LC_ALL, "");
    int n = mbstowcs(NULL, ansi.data(), 0);
    wchar_t *s = new wchar_t[n + 1];
    mbstowcs(s, ansi.data(), n + 1);
    s[n] = '\0';
    std::wstring dst(s);
    setlocale(LC_ALL, locale);
#endif
    delete[] s;
    return dst;
}

std::string wcs_to_ansi(const std::wstring &utf16)
{
#ifdef _WIN32
    int n = WideCharToMultiByte(CP_ACP, 0, utf16.data(), -1, NULL, 0, 0, FALSE);
    char *s = new char[n + 1];
    WideCharToMultiByte(CP_ACP, 0, utf16.data(), -1, s, n + 1, 0, FALSE);
    s[n] = '\0';
    std::string dst(s);
#else
    char *locale = setlocale(LC_ALL, "");
    int n = wcstombs(NULL, utf16.data(), 0);
    char *s = new char[n + 1];
    wcstombs(s, utf16.data(), n + 1);
    s[n] = '\0';
    std::string dst(s);
    setlocale(LC_ALL, locale);
#endif
    delete[] s;
    return dst;
}

static int32_t utf_8to32(uint32_t *utf32, const uint8_t *utf8, uint32_t len)
{
    int32_t ret;

    if (len == 0) {
        ret = 0;
    }
    else if (*utf8 < 0x80) {
        if (utf32 != NULL) {
            *utf32 = *utf8;
        }
        ret = 1;
    }
    else if (*utf8 < 0xC0) {
        ret = -1;
    }
    else if (*utf8 < 0xE0) {
        if (len < 2) {
            ret = -1;
        }
        else if (*(utf8 + 1) > 0xBF) {
            ret = -1;
        }
        else {
            if (utf32 != NULL) {
                *utf32 = (((uint32_t)*utf8 & 0x1F) << 6) |
                    ((uint32_t)*(utf8 + 1) & 0x3F);
            }
            ret = 2;
        }
    }
    else if (*utf8 < 0xF0) {
        if (len < 3) {
            ret = -1;
        }
        else if (*(utf8 + 1) > 0xBF || *(utf8 + 2) > 0xBF) {
            ret = -1;
        }
        else {
            if (utf32 != NULL) {
                *utf32 = (((uint32_t)*utf8 & 0x1F) << 12) |
                    (((uint32_t)*(utf8 + 1) & 0x3F) << 6) |
                    ((uint32_t)*(utf8 + 2) & 0x3F);
            }
            ret = 3;
        }
    }
    else if (*utf8 < 0xF8) {
        if (len < 4) {
            ret = -1;
        }
        else if (*(utf8 + 1) > 0xBF || *(utf8 + 2) > 0xBF ||
                 *(utf8 + 3) > 0xBF) {
            ret = -1;
        }
        else {
            if (utf32 != NULL) {
                *utf32 = (((uint32_t)*utf8 & 0xF) << 18) |
                    (((uint32_t)*(utf8 + 1) & 0x3F) << 12) |
                    (((uint32_t)*(utf8 + 2) & 0x3F) << 6) |
                    ((uint32_t)*(utf8 + 3) & 0x3F);
            }
            ret = 4;
        }
    }
    else if (*utf8 < 0xFC) {
        if (len < 5) {
            ret = -1;
        }
        else if (*(utf8 + 1) > 0xBF || *(utf8 + 2) > 0xBF ||
                 *(utf8 + 3) > 0xBF || *(utf8 + 4) > 0xBF) {
            ret = -1;
        }
        else {
            if (utf32 != NULL) {
                *utf32 = (((uint32_t)*utf8 & 0x7) << 24) |
                    (((uint32_t)*(utf8 + 1) & 0x3F) << 18) |
                    (((uint32_t)*(utf8 + 2) & 0x3F) << 12) |
                    (((uint32_t)*(utf8 + 3) & 0x3F) << 6) |
                    ((uint32_t)*(utf8 + 4) & 0x3F);
            }
            ret = 5;
        }
    }
    else if (*utf8 < 0xFE) {
        if (len < 6) {
            ret = -1;
        }
        else if (*(utf8 + 1) > 0xBF || *(utf8 + 2) > 0xBF ||
                 *(utf8 + 3) > 0xBF ||
                 *(utf8 + 4) > 0xBF || *(utf8 + 5) > 0xBF) {
            ret = -1;
        }
        else {
            if (utf32 != NULL) {
                *utf32 = (((uint32_t)*utf8 & 0x3) << 30) |
                    (((uint32_t)*(utf8 + 1) & 0x3F) << 24) |
                    (((uint32_t)*(utf8 + 2) & 0x3F) << 18) |
                    (((uint32_t)*(utf8 + 3) & 0x3F) << 12) |
                    (((uint32_t)*(utf8 + 4) & 0x3F) << 6) |
                    ((uint32_t)*(utf8 + 5) & 0x3F);
            }
            ret = 6;
        }
    }
    else {
        ret = -1;
    }
    return ret;
}

static int32_t utf_32to8(uint8_t *utf8, uint32_t utf32)
{
    int32_t ret;

    if (utf32 < 0x80) {
        if (utf8 != NULL) {
            *utf8 = (uint8_t)utf32;
        }
        ret = 1;
    }
    else if (utf32 < 0x800) {
        if (utf8 != NULL) {
            *utf8 = 0xC0 | (utf32 >> 6);
            *(utf8 + 1) = 0x80 | (utf32 & 0x3F);
        }
        ret = 2;
    }
    else if (utf32 < 0x10000) {
        if (utf8 != NULL) {
            *utf8 = 0xE0 | (utf32 >> 12);
            *(utf8 + 1) = 0x80 | ((utf32 >> 6) & 0x3F);
            *(utf8 + 2) = 0x80 | (utf32 & 0x3F);
        }
        ret = 3;
    }
    else if (utf32 < 0x200000) {
        if (utf8 != NULL) {
            *utf8 = 0xF0 | (utf32 >> 18);
            *(utf8 + 1) = 0x80 | ((utf32 >> 12) & 0x3F);
            *(utf8 + 2) = 0x80 | ((utf32 >> 6) & 0x3F);
            *(utf8 + 3) = 0x80 | (utf32 & 0x3F);
        }
        ret = 4;
    }
    else if (utf32 < 0x4000000) {
        if (utf8 != NULL) {
            *utf8 = 0xF8 | (utf32 >> 24);
            *(utf8 + 1) = 0x80 | ((utf32 >> 18) & 0x3F);
            *(utf8 + 2) = 0x80 | ((utf32 >> 12) & 0x3F);
            *(utf8 + 3) = 0x80 | ((utf32 >> 6) & 0x3F);
            *(utf8 + 4) = 0x80 | (utf32 & 0x3F);
        }
        ret = 5;
    }
    else if (utf32 < 0x80000000) {
        if (utf8 != NULL) {
            *utf8 = 0xFC | (utf32 >> 30);
            *(utf8 + 1) = 0x80 | ((utf32 >> 24) & 0x3F);
            *(utf8 + 2) = 0x80 | ((utf32 >> 18) & 0x3F);
            *(utf8 + 3) = 0x80 | ((utf32 >> 12) & 0x3F);
            *(utf8 + 4) = 0x80 | ((utf32 >> 6) & 0x3F);
            *(utf8 + 5) = 0x80 | (utf32 & 0x3F);
        }
        ret = 6;
    }
    else {
        ret = -1;
    }
    return ret;
}

static int32_t utfs_8to32(uint32_t *utf32, const uint8_t *utf8, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_8to32(utf32, utf8, len);
        if (n < 0) {
            total = -1;
            break;
        }
        if (utf32 != NULL) {
            utf32++;
        }
        total++;
        utf8 += n;
        len -= n;
    }
    return total;
}

static int32_t utfs_32to8(uint8_t *utf8, const uint32_t *utf32, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_32to8(utf8, *utf32);
        if (n < 0) {
            total = -1;
            break;
        }
        total += n;
        if (utf8 != NULL) {
            utf8 += n;
        }
        utf32++;
        len--;
    }
    return total;
}

static int32_t utf_16to32(uint32_t *utf32, const uint16_t *utf16, uint32_t len)
{
    int32_t ret;

    if (len == 0) {
        ret = 0;
    }
    else if ((*utf16 ^ 0xD800) < 0x400) {
        if (len >= 2 && (*(utf16 + 1) ^ 0xDC00) < 0x400) {
            if (utf32 != NULL) {
                *utf32 = (*utf16 ^ 0xD800) << 10;
                *utf32 |= *(utf16 + 1) ^ 0xDC00;
                *utf32 += 0x10000;
            }
            ret = 2;
        }
        else {
            ret = -1;
        }
    }
    else if ((*utf16 ^ 0xDC00) < 0x400) {
        ret = -1;
    }
    else {
        if (utf32 != NULL) {
            *utf32 = *utf16;
        }
        ret = 1;
    }
    return ret;
}

static int32_t utf_32to16(uint16_t *utf16, uint32_t utf32)
{
    int32_t ret;

    if (utf32 < 0x10000) {
        if (utf16 != NULL) {
            *utf16 = (uint16_t)utf32;
        }
        ret = 1;
    }
    else if (utf32 <= 0x10FFFF) {
        if (utf16 != NULL) {
            utf32 -= 0x10000;
            *utf16 = (utf32 >> 10) | 0xD800;
            *(utf16 + 1) = (utf32 & 0x3FF) | 0xDC00;
        }
        ret = 2;
    }
    else {
        ret = -1;
    }
    return ret;
}

static int32_t utf_8to16(uint16_t *utf16, const uint8_t *utf8, uint32_t len)
{
    uint32_t utf32;
    int32_t n;

    n = utf_8to32(&utf32, utf8, len);
    if (n > 0) {
        utf_32to16(utf16, utf32);
    }
    return n;
}

static int32_t utf_16to8(uint8_t *utf8, const uint16_t *utf16, uint32_t len)
{
    uint32_t utf32;
    int32_t n;

    n = utf_16to32(&utf32, utf16, len);
    if (n > 0) {
        n = utf_32to8(utf8, utf32);
    }
    return n;
}

/*
 * When utf8 bytes is larger than 3, unicode is greater than 0xFFFF,
 * and utf16 should use 2 bytes.
 */
int32_t utfs_8to16(uint16_t *utf16, const uint8_t *utf8, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_8to16(utf16, utf8, len);
        if (n < 0) {
            total = -1;
            break;
        }
        if (utf16 != NULL) {
            if (n <= 3) {
                utf16++;
            }
            else {
                utf16 += 2;
            }
        }
        if (n <= 3) {
            total++;
        }
        else {
            total += 2;
        }
        utf8 += n;
        len -= n;
    }
    return total;
}

/*
 * When utf8 bytes is larger than 3, unicode is greater than 0xFFFF,
 * and utf16 should use 2 bytes.
 */
int32_t utfs_16to8(uint8_t *utf8, const uint16_t *utf16, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_16to8(utf8, utf16, len);
        if (n < 0) {
            total = -1;
            break;
        }
        total += n;
        if (utf8 != NULL) {
            utf8 += n;
        }
        if (n <= 3) {
            utf16++;
            len--;
        }
        else {
            utf16 += 2;
            len -= 2;
        }
    }
    return total;
}

int32_t utfs_16to32(uint32_t *utf32, const uint16_t *utf16, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_16to32(utf32, utf16, len);
        if (n < 0) {
            total = -1;
            break;
        }
        if (utf32 != NULL) {
            utf32++;
        }
        total++;
        utf16 += n;
        len -= n;
    }
    return total;
}

int32_t utfs_32to16(uint16_t *utf16, const uint32_t *utf32, uint32_t len)
{
    int32_t n;
    int32_t total;

    total = 0;
    while (len > 0) {
        n = utf_32to16(utf16, *utf32);
        if (n < 0) {
            total = -1;
            break;
        }
        total += n;
        if (utf16 != NULL) {
            utf16 += n;
        }
        utf32++;
        len--;
    }
    return total;
}

std::wstring utf8_to_wcs(const std::string &utf8)
{
#ifdef _WIN32
    int n = utfs_8to16(NULL, (const uint8_t*)utf8.data(), utf8.size());
    uint16_t *s = new uint16_t[n + 1];
    utfs_8to16(s, (const uint8_t*)utf8.data(), utf8.size());
    s[n] = '\0';
    std::wstring dst((const wchar_t*)s);
    delete[] s;
    return dst;
#else
    int n = utfs_8to32(NULL, (const uint8_t*)utf8.data(), utf8.size());
    uint32_t *s = new uint32_t[n + 1];
    utfs_8to32(s, (const uint8_t*)utf8.data(), utf8.size());
    s[n] = '\0';
    std::wstring dst((const wchar_t*)s);
    delete[] s;
    return dst;
#endif
}

std::string wcs_to_utf8(const std::wstring &utf16)
{
#ifdef _WIN32
    int n = utfs_16to8(NULL, (const uint16_t*)utf16.data(), utf16.size());
    char *s = new char[n + 1];
    utfs_16to8((uint8_t*)s, (const uint16_t*)utf16.data(), utf16.size());
    s[n] = '\0';
    std::string dst(s);
    delete[] s;
    return dst;
#else
    int n = utfs_32to8(NULL, (const uint32_t*)utf16.data(), utf16.size());
    char *s = new char[n + 1];
    utfs_32to8((uint8_t*)s, (const uint32_t*)utf16.data(), utf16.size());
    s[n] = '\0';
    std::string dst(s);
    delete[] s;
    return dst;
#endif
}

std::wstring convert_utf_to_unicode(const std::string& utf_string)
{
	std::wstring unicode_string;
	int i = 0;
	wchar_t word_unicode;

	const uint8_t *utf_data = (const uint8_t *)utf_string.data();
	for (i = 0; i < utf_string.size();)
	{
		word_unicode = 0;
		if ((uint8_t)utf_data[i] < 0x80)
		{
			word_unicode = utf_data[i];
			i++;
		}
		else if (((uint8_t)utf_data[i] & 0xe0) == 0xc0 && (utf_data[i + 1] & 0x80) == 0x80)
		{
			word_unicode = (utf_data[i] & 0x1f) << 6;
			word_unicode = word_unicode + (utf_data[i + 1] & 0x3f);
			i += 2;
		}
		else if ((utf_data[i] & 0xf0) == 0xe0 && (utf_data[i + 1] & 0x80) == 0x80 && (utf_data[i + 2] & 0x80) == 0x80)
		{
			word_unicode = (int)((utf_data[i] & 0x0f) << 12);
			word_unicode = word_unicode + (wchar_t)((utf_data[i + 1] & 0x3f) << 6);
			word_unicode = word_unicode + ((utf_data[i + 2] & 0x3f));

			i += 3;
		}
		else
		{
			unicode_string.clear();
			break;
		}
		unicode_string.push_back(word_unicode);
	}

	return unicode_string;
}

int wide_char_string_to_int(const std::wstring& wide_char_string)
{
	int value = 0;

	value = 0;
	for (int i = 0; i < wide_char_string.length(); i++)
	{
		if (wide_char_string[i] >= 48 && wide_char_string[i] <= 57)
		{
			value = value * 10 + (wide_char_string[i] - 48);
		}
		else
		{
			break;
		}
	}
	return value;
}
