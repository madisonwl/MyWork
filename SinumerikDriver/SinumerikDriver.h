// SinumerikDriver.h : SinumerikDriver DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSinumerikDriverApp
// �йش���ʵ�ֵ���Ϣ������� SinumerikDriver.cpp
//

class CSinumerikDriverApp : public CWinApp
{
public:
	CSinumerikDriverApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
