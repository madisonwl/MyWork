#pragma once
enum DebugInforType
{
	Driver,
	Test,
	Debug
};
class CDebugInfor
{
public:
	CDebugInfor();
	~CDebugInfor();


public:
	static void ShowDebugInfor(int nDebugType, TCHAR* szDebugInfor);
};

