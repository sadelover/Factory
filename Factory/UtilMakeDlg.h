
#pragma once

#include <string>

namespace  UtilMakeDlg
{
    std::wstring    GetFormatFromString(const std::wstring& str);
    UINT            ConvertStringToNumber(const std::wstring& str);
    UINT            ConvertHexStringToNumber(const std::wstring& str);
    int             GetNumberFromString(const std::wstring& str);
    std::wstring      GetFormatStringFromWstring(const std::wstring& str,  UINT idx);
    std::wstring        NumberToWstring(int number);
    bool            FindStringInString(const std::wstring& strSrc0, const std::wstring& strToFind0);
	void            HEXNumberToString(unsigned int number, std::string & str);
}
