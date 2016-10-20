#include "stdafx.h"
#include "DebugInfor.h"

#define WM_DEBUGINFOR (WM_USER +100)
CDebugInfor::CDebugInfor()
{
}


CDebugInfor::~CDebugInfor()
{
}

void CDebugInfor::ShowDebugInfor(int nDebugType, TCHAR* szDebugInfor)
{
	HWND hwnd = FindWindow(NULL, _T("工程调试信息"));
	int nlength = sizeof(szDebugInfor);            //strlen(szDebugInfor);
	if (nlength>128)
	{
		nlength = 128;
	}
	TCHAR szMess[128];//
	memset(szMess, 0, sizeof(szMess));
	wcscpy_s(szMess, szDebugInfor);
	
	ATOM  messAtom = GlobalAddAtom(szMess);
	ASSERT(messAtom);
	if (hwnd)
	{
		::SendMessage(hwnd, WM_DEBUGINFOR, (WPARAM)messAtom, 1);
	}
	GlobalDeleteAtom(messAtom);
}
