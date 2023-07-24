
#include "stdafx.h"
#include "UtilMakeDlg.h"
#include <strstream>

#include "Tools/Util/UtilString.h"

//#include "util/UtilsFile.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
namespace UtilMakeDlg
{
    string WChar2Ansi(LPCWSTR pwszSrc)
    {
        // Get the wide char length.
        int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
        if (nLen <= 0)
        {
            return std::string("");
        }

        char* pszDst = new char[nLen];
        if (NULL == pszDst) 
        {
            return std::string("");
        }

        // Change the wide charmulti byte to multi byte.
        WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
        pszDst[nLen - 1] = 0;

        std::string strTemp(pszDst);

        delete [] pszDst;
        return strTemp;
    }

    //replace number in string to %d
    wstring GetFormatFromString(const wstring& str)
    {
        wstring strRet;
        CString strTemp;

        int number = GetNumberFromString(str);
        if(number < 0){
            //has no number
            return str;
        }
        //strTemp.Format(_T("%d"), number );
        const CString strNum = NumberToWstring(number).c_str();

        const int numPos = (int)str.find(strNum);
        if(-1 == numPos
            || numPos >= (int)str.length() ){
                //can't find the number
                ASSERT(false);
                return str;
        }
        strRet = str.substr(0, numPos);
        strRet += _T("%d");
        strRet += str.substr(numPos + strNum.GetLength()  );
        //strRet = strPart0 + strPart1;   

        return strRet;
    }

    //strtod
    UINT ConvertStringToNumber(const wstring& str)
    {
        if(_T(" ") == str){
            return 0;
        }
        string str0= WChar2Ansi(str.c_str() );  
        int num = atoi( str0.c_str() );
        return num;
    }

    //sscanf("a0", "%02X", &num1);
    //get number from string like "0x84acef"
    UINT ConvertHexStringToNumber(const std::wstring& wstrOld)
    {
        UINT num = 0;
        //vector<TCHAR> tcharArray;
        const wstring strHex = _T("0123456789abcdefxxxxx");
        CString cstr = wstrOld.c_str();
        cstr.MakeLower();
        wstring wstrLow = cstr;
        const UINT strLength = (UINT) wstrLow.length();
        int pos = -1;
        TCHAR ch;
        for(UINT idx =0; idx < strLength; idx++){
            ch = wstrLow.at(idx);
            pos = (int)strHex.find(ch);
            if(pos< 0
                || pos >= (int)strHex.length() ){
                    ASSERT(false);
                    continue;
            }
            else if(pos >15){ }
            else{
                num = num*16 + pos;
            }
        }
        return num;
    }

    //max to 20
    int GetNumberFromString(const wstring& str)
    {
        UINT numberRet = -1;
        CString strTemp;
        const UINT strLen = (UINT)str.length();
        int posStart = -1;
        int posEnd = -1;
        for(UINT idx= 0; idx< strLen; idx++){
            if(str[idx] >= _T('0') && str[idx] <= _T('9') ){
                if(posStart < 0){ //1st char
                    posStart = idx;               
                }
                else{
                }
                posEnd = idx;
            }
            else if(posStart >= 0){
                //end char
                break;
            }
            else{}
        }

        if(posStart < 0){
            //find no number
            return numberRet;
        }

        ASSERT(posEnd - posStart + 1 >= 0 );
        wstring wstrSub = str.substr(posStart, posEnd - posStart + 1);
        string strSub = UtilString::ConvertWideCharToMultiByte(wstrSub);
        numberRet = atoi(strSub.c_str() );

        return numberRet;
    }

    wstring    GetFormatStringFromWstring(const wstring& wstr, UINT idx)
    {
        CString cstr;
        cstr.Format(wstr.c_str(), idx);
        //return UtilString::ConvertWideCharToMultiByte(cstr.GetString() );
        return cstr.GetString();
    }

    std::wstring        NumberToWstring(int number)
    {
        CString cstr;
        cstr.Format(_T("%d"), number);
        //return UtilString::ConvertWideCharToMultiByte(cstr.GetString() );
        return cstr.GetString();
    }

    bool FindStringInString(const wstring& strSrc0, const wstring& strToFind0)
    {
        bool ifFind = false;
        CString strSrc1 = strSrc0.c_str();
        strSrc1.MakeLower();
        CString strToFind1 = strToFind0.c_str();
        strToFind1.MakeLower();


        int findPos = (int)strSrc1.Find(strToFind1);
        if(findPos >=0 && findPos < (int)strSrc1.GetLength() )
            ifFind = true;
        return ifFind;
    }

	void   HEXNumberToString( unsigned int number, std::string & str )
	{
		std::strstream buffer;
		buffer.setf(std::ios::showbase);
		buffer <<std::hex << number;   
		buffer >> str;
	}
}
