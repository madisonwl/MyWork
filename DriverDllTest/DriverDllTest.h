
// DriverDllTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDriverDllTestApp: 
// �йش����ʵ�֣������ DriverDllTest.cpp
//

class CDriverDllTestApp : public CWinApp
{
public:
	CDriverDllTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDriverDllTestApp theApp;