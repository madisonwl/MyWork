
#pragma once
#include "INetMach.h"
#include "SinumerikThread.h"
#include "NetMachDriverData.h"
//#include "DataCallBack.h"
#include "SinumerikDriver_h.h"
// CSinumerikDriverSer 命令目标

class CSinumerikDriverSer : public CCmdTarget
{
	DECLARE_DYNCREATE(CSinumerikDriverSer)

public:
	CSinumerikDriverSer();
	virtual ~CSinumerikDriverSer();

	virtual void OnFinalRelease();
	CSinumerikThread* m_pThread;
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSinumerikDriverSer)
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(NetMach1, INetMach)
	STDMETHOD_(BOOL, Start)(const CHAR* szMachIP, int nPort);
	STDMETHOD_(BOOL, Stop)();
	STDMETHOD_(CString, GetSingleVarValue)(const CHAR* sVarFlag);   //通过标签变量查找值
	STDMETHOD_(CString, GetSingleVarValue)(const int nIndex);       //通过索引找值
	STDMETHOD_(BOOL, GetVarMapData)(CVarMapData* pData);

	STDMETHOD_(HRESULT,Advise)(ISinuDataCallBack * pCallBack, long * pdwCookie);
	STDMETHOD_(HRESULT,Unadvise)(long dwCookie);
	STDMETHOD_(HRESULT,Add)(long n1, long n2);

	END_INTERFACE_PART(NetMach1)

public:
	BOOL StartMonitor(const CHAR* szMachIP, int nPort);
	BOOL GetDeviceData();
	BOOL StopMonitor();
	BOOL GetVarData(/*in*/const CHAR* szVarFlag,/*out*/CString &srVakue,/*out*/CString &srValueType);//根据标签名称获得标签的值和值的数据类型
	BOOL GetVarData(/*in*/const int nVarIndex,/*out*/CString &srVakue,/*out*/CString &srValueType);   //通过索引查找值
	BOOL GetVarMapData(CVarMapData* pData);

	HRESULT Advise(ISinuDataCallBack * pCallBack, long * pdwCookie);
	HRESULT Unadvise(long dwCookie);

	HRESULT Add(long n1, long n2);
private:
	BOOL m_bStopGetData;
	static UINT GatData(LPVOID lpContext);

private:
	ISinuDataCallBack* m_pCallBack[10];

protected:
public:
	STDMETHOD(Fire_Test)(LONG nSend);
};


