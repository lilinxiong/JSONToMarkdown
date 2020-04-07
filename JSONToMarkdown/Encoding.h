#ifndef ENCODING_H
#define ENCODING_H

#include <string>

void convert_content(std::wstring & wstr);

void convert_content(wchar_t & w);

std::wstring ansi_to_wcs(const std::string &ansi);

std::string wcs_to_ansi(const std::wstring &utf16);

std::wstring utf8_to_wcs(const std::string &utf8);

std::string wcs_to_utf8(const std::wstring &utf16);

std::wstring convert_utf_to_unicode(const std::string& utf_string);

int wide_char_string_to_int(const std::wstring& wide_char_string);

#endif // ENCODING
