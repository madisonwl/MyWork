#pragma once

#include "SinumerikOperateSocket.h"
#include <map>
#include "NetMachDriverData.h"

// CSinumerikThread
class CSinumerikThread : public CWinThread
{
	DECLARE_DYNCREATE(CSinumerikThread)

protected:
	CSinumerikThread();           // 动态创建所使用的受保护的构造函数
	

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ~CSinumerikThread();
public:
	CString m_IP;
	int m_nPort;
	CSinumerikOperateSocket m_SinumerikSocket;
	BOOL ConnectServer();
	BOOL DisConnect();
	void SetStopFlag(BOOL bStopFlag);
	SinumerikOperateData m_SimData;
	BOOL GetNCData();
	CString FindVarValue(const CHAR* sVarFlag);
	CString FindVarValue(const int nVarIdex);
	BOOL GetVarMapData(CVarMapData* pData);
private:
	void InitMap();
	BOOL GetServVersion();  //获得服务端版本号
	BOOL GetVarInfor();
	BOOL GetVarTypeDefine();
	BOOL GetMachData();
	BOOL GetVarOffSet();
	void CopyDataToMap();
private:
	BOOL m_StopFlag;
	std::map<CString, CString> m_VarvalueMap;  //存储变量名和值
	BOOL m_bMsgBusy;
	BOOL m_bServVersion;
	BOOL m_bVarInfor;
	BOOL m_bVarDefine;
	BOOL m_bGetData;
	BOOL m_bOffSet;
	BOOL m_beConnected;
	int m_nCountTimes;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
};


