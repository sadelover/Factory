#include "StdAfx.h"
#include "WarningEntry.h"


Beopdatalink::CWarningEntry::~CWarningEntry(void)
{
}




Beopdatalink::CWarningEntry::CWarningEntry()
{
	GetLocalTime(&m_time);
	m_warningcode = -1;
	m_warninglevel = 0;
	m_bConfirmed = false;
	m_nConfirmed = 0;
}

SYSTEMTIME Beopdatalink::CWarningEntry::GetTimestamp() const
{
	return m_time;
}

void Beopdatalink::CWarningEntry::SetTimestamp( const SYSTEMTIME& stime )
{
	m_time = stime;
}


SYSTEMTIME Beopdatalink::CWarningEntry::GetEndTimestamp() const
{
	return m_timeEnd;
}

void Beopdatalink::CWarningEntry::SetEndTimestamp( const SYSTEMTIME& stime )
{
	m_timeEnd = stime;
}

wstring	Beopdatalink::CWarningEntry::GetConfirmedUser() const
{
	return m_strConfirmedUser;
}


void	Beopdatalink::CWarningEntry::SetConfirmedUser(const wstring& strUser)
{
	m_strConfirmedUser = strUser;
}

bool    Beopdatalink::CWarningEntry::IsConfirmed() const
{
	return m_bConfirmed;
}


void    Beopdatalink::CWarningEntry::SetConfirmed(const bool &bConfirmed)
{
	m_bConfirmed = bConfirmed;
}

wstring Beopdatalink::CWarningEntry::GetWarningInfo() const
{
	return m_warninginfo;
}

void Beopdatalink::CWarningEntry::SetWarningInfo( const wstring& warninginfo )
{
	m_warninginfo = warninginfo;
}

int Beopdatalink::CWarningEntry::GetWarningCode() const
{
	return m_warningcode;
}

void Beopdatalink::CWarningEntry::SetWarningCode( int warningcode )
{
	m_warningcode = warningcode;
}

int Beopdatalink::CWarningEntry::GetWarningLevel() const
{
	return m_warninglevel;
}

void Beopdatalink::CWarningEntry::SetWarningLevel( int warninglevel )
{
	m_warninglevel = warninglevel;
}

int Beopdatalink::CWarningEntry::GetConfirmedType() const
{
	return m_nConfirmed;
}

void Beopdatalink::CWarningEntry::SetConfirmedType( const int &nConfirmed )
{
	m_nConfirmed = nConfirmed;
}
