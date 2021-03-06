
// DriverDllTestDlg.h : 头文件
//

#pragma once
#include "INetMach.h"
#include "TestCallBack.h"
#include "afxwin.h"

// CDriverDllTestDlg 对话框
class CDriverDllTestDlg : public CDialogEx
{
// 构造
public:
	CDriverDllTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRIVERDLLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSinumerik();
	void Init();
	INetMach* pNetMach;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonGetall();
	CVarMapData* pData;
	afx_msg void OnBnClickedButton2();
	CTestCallBack m_Testcallbcak;
	DWORD m_dwCookie;
	afx_msg void OnBnClickedButton3();
	CEdit m_edit;
};
