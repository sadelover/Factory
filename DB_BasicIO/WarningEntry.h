#pragma once

#include "wtypes.h"
#include "xstring"
#include "atlcomtime.h"

using namespace std;


class CEqmDrawPage;
class CEqmDrawText;
namespace Beopdatalink
{

	class _declspec(dllexport) CWarningEntry
	{
	public:
	public:
		CWarningEntry();
		~CWarningEntry();

		SYSTEMTIME GetTimestamp() const;
		void	SetTimestamp(const SYSTEMTIME& stime);
		SYSTEMTIME GetEndTimestamp() const;
		void	SetEndTimestamp(const SYSTEMTIME& stime);

		wstring	GetWarningInfo() const;
		void	SetWarningInfo(const wstring& warninginfo);

		wstring	GetConfirmedUser() const;
		void	SetConfirmedUser(const wstring& strUser);

		bool    IsConfirmed() const;
		void    SetConfirmed(const bool &bConfirmed);

		int		GetConfirmedType() const;
		void    SetConfirmedType(const int &nConfirmed);

		int	GetWarningCode() const;
		void	SetWarningCode(int warningcode);

		int	GetWarningLevel() const;
		void	SetWarningLevel(int warninglevel);

	private:
		SYSTEMTIME m_time;
		int	m_warningcode;
		wstring m_warninginfo;
		int m_warninglevel;
		SYSTEMTIME m_timeEnd;
		wstring m_strConfirmedUser;
		bool m_bConfirmed;
		int	 m_nConfirmed;

	public:
		//实时运行时的用户操作记录
		COleDateTime m_timeConfirmed;
		COleDateTime m_timeShieldTo;

		wstring m_strBindPointName;

	};

}
