
#pragma once
#include "INetMach.h"
#include "SinumerikThread.h"
#include "NetMachDriverData.h"
// CSinumerikDriverSer ����Ŀ��

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
	STDMETHOD_(CString, GetSingleVarValue)(const CHAR* sVarFlag);   //ͨ����ǩ��������ֵ
	STDMETHOD_(CString, GetSingleVarValue)(const int nIndex);       //ͨ��������ֵ
	STDMETHOD_(BOOL, GetVarMapData)(CVarMapData* pData);
	END_INTERFACE_PART(NetMach1)

public:
	BOOL StartMonitor(const CHAR* szMachIP, int nPort);
	BOOL GetDeviceData();
	BOOL StopMonitor();
	BOOL GetVarData(/*in*/const CHAR* szVarFlag,/*out*/CString &srVakue,/*out*/CString &srValueType);//���ݱ�ǩ���ƻ�ñ�ǩ��ֵ��ֵ����������
	BOOL GetVarData(/*in*/const int nVarIndex,/*out*/CString &srVakue,/*out*/CString &srValueType);   //ͨ����������ֵ
	BOOL GetVarMapData(CVarMapData* pData);
private:
	BOOL m_bStopGetData;
	static UINT GatData(LPVOID lpContext);
};

