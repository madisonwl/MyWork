#if !defined(AFX_SINUMERIKOPERATESOCKET_H__4A52E9C9_1D4F_46B1_AD49_61EDBA25A899__INCLUDED_)
#define AFX_SINUMERIKOPERATESOCKET_H__4A52E9C9_1D4F_46B1_AD49_61EDBA25A899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxsock.h>
/*#include<LIST.H>*/
#include <afxtempl.h>
#include "SinumerikOperateData.h"
//



/////////////////////////////////////////////////////////////////////////////
// CSinumerikOperateSocket command target


class CSinumerikOperateSocket : public CAsyncSocket
{

public:
	CSinumerikOperateSocket();
	virtual ~CSinumerikOperateSocket();


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSinumerikOperateSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSinumerikOperateSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
public:
	BOOL ConnectServer(const CString& sServerIP, const int nPort);	
	void Disconnect();
	BOOL GetServVersion();                    //获得服务端版本号
	BOOL GetCNCVarinfor();                    //获得变量标签号
	BOOL GetVarTypeDefine();                  //获得变量类型定义
	BOOL GetVarOffSet();                      //获得变量偏移
	BOOL GetNCData();

public:
	BOOL m_bConnected;
	CTypedPtrList<CPtrList, SimVarPara*> SimVarParaList;

	CString m_strSerVersion;  //驱动版本版本号,形式1
	double m_dbSerVersion;    //驱动版本版本号，形式2
	int m_nAlloffSet;		  //总的偏移量（解析发送）
	SinumerikOperateData* m_pSimOperdata;

private:
	void ProcessMsg(const int nByte);
	void AnalyseMsgVersion(const char* buff);
	void AnalyseMsgData(const char* buff);
	BOOL AnalyseProductinfor(const char* buff);

	BOOL GetAllVarValue(const CTypedPtrList<CPtrList, SimVarPara*> &SimVarParaList, const char* buff);

	void SaveVarFlag(const char* buff);
	void SaveVarOffSet(const char* buff);
	void SaveVarDefine(const char* buff);
	void SaveVarValue(const char* buff);

	BOOL SendReadCmd(const CString strCmd);   //向服务端发送读取指令
	

	void FreeList();
	CString ComPoundAlarm();
	int GetSocketErrorText();
protected:
	//数据缓存区
	char* m_pcBuffer;
	char* tempHeadbuff;
	char* tempDatabuff;
	char* tempvarbuff;
	
	SimVarData * m_pSimVarData;
	BOOL m_bGetValue;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINUMERIKOPERATESOCKET_H__4A52E9C9_1D4F_46B1_AD49_61EDBA25A899__INCLUDED_)
