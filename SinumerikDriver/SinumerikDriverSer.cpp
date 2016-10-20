// SinumerikDriverSer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SinumerikDriver.h"
#include "SinumerikDriverSer.h"
#include "DebugInfor.h"

#include "NetMachDriverData.h"




// CSinumerikDriverSer
IMPLEMENT_DYNCREATE(CSinumerikDriverSer, CCmdTarget)

CSinumerikDriverSer::CSinumerikDriverSer()
{
	EnableAutomation();
	
	// Ϊ��ʹӦ�ó����� OLE �Զ��������ڻ״̬ʱ����
	//	���У����캯������ AfxOleLockApp��
	m_pThread = NULL;
	m_bStopGetData = FALSE;
	AfxOleLockApp();
}

CSinumerikDriverSer::~CSinumerikDriverSer()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	delete m_pThread;
	m_pThread = NULL;
	AfxOleUnlockApp();
}


void CSinumerikDriverSer::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}



BEGIN_MESSAGE_MAP(CSinumerikDriverSer, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CSinumerikDriverSer, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ������� IID_ISinumerikDriverSer ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {8E729AA6-9E85-45E2-BC41-76FAE56EC135}
static const IID IID_ISinumerikDriverSer =
{ 0x8E729AA6, 0x9E85, 0x45E2, { 0xBC, 0x41, 0x76, 0xFA, 0xE5, 0x6E, 0xC1, 0x35 } };

BEGIN_INTERFACE_MAP(CSinumerikDriverSer, CCmdTarget)
	INTERFACE_PART(CSinumerikDriverSer, IID_ISinumerikDriverSer, Dispatch)
	INTERFACE_PART(CSinumerikDriverSer, IID_NetMach1, NetMach1) // Add-wl-2016818
END_INTERFACE_MAP()

// {4CB51EDE-4B17-4ED2-9CD7-D62F02FB9E32}
IMPLEMENT_OLECREATE_FLAGS(CSinumerikDriverSer, "SinumerikDriver.SinumerikDriverSer", afxRegApartmentThreading, 0x4cb51ede, 0x4b17, 0x4ed2, 0x9c, 0xd7, 0xd6, 0x2f, 0x2, 0xfb, 0x9e, 0x32)


// CSinumerikDriverSer ��Ϣ�������
STDMETHODIMP_(ULONG) CSinumerikDriverSer::XNetMach1::AddRef(void)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	return pThis->ExternalAddRef();
}
//
STDMETHODIMP_(ULONG) CSinumerikDriverSer::XNetMach1::Release(void)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	return pThis->ExternalRelease();
}
//Query the com interface, not to deal 
STDMETHODIMP CSinumerikDriverSer::XNetMach1::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}


BOOL CSinumerikDriverSer::StartMonitor(const CHAR* szMachIP, int nPort)
{
	if (!m_pThread)
	{
		m_pThread = (CSinumerikThread*)AfxBeginThread(RUNTIME_CLASS(CSinumerikThread),
			THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		CDebugInfor::ShowDebugInfor(Driver, _T("�����ɼ��߳�"));
		if (m_pThread)
		{
			m_pThread->ResumeThread();
			m_pThread->m_IP = szMachIP;
			m_pThread->m_nPort = nPort;
			m_pThread->ConnectServer();
			AfxBeginThread(GatData, this);
			return TRUE;
		}
		else
		{
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CSinumerikDriverSer::GetDeviceData()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("��ȡ�豸����"));
	return FALSE;
}

BOOL CSinumerikDriverSer::StopMonitor()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("�رղɼ��߳�"));
	m_bStopGetData = TRUE;  //ֹͣ
	if (m_pThread)
	{
		m_pThread->DisConnect();
		Sleep(100);
		{
			m_pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			m_pThread->PostThreadMessage(WM_QUIT, NULL, NULL);
			CDebugInfor::ShowDebugInfor(Driver, _T("�رղɼ��̳߳ɹ�"));
		}
		try
		{
			WaitForSingleObject(m_pThread->m_hThread, 5000);
		}
		catch (...)
		{
		}
		m_pThread = NULL;
	}
	return FALSE;
}

UINT CSinumerikDriverSer::GatData(LPVOID lpContext)
{
	CSinumerikDriverSer *pSer = (CSinumerikDriverSer*)lpContext;
	while (!(pSer->m_bStopGetData)&& pSer->m_pThread!=NULL)
	{
		pSer->m_pThread->GetNCData();
		Sleep(1000);
	}
	return 0;
}
//********************************************************************
//* �������ƣ�GetVarData
//* �������ܣ���ȡ��ǩ��ֵ
//* ����ֵ�� TRUE���ɹ���FALSE��ʧ��
//* ����˵����
//* �����ߣ�  dyx-wl
//* �������ڣ�2016��10��13��
//* �޸����ڣ�
//* �޸�˵����
//********************************************************************
//
BOOL CSinumerikDriverSer::GetVarData(/*in*/const CHAR* szVarFlag,/*out*/CString &srVakue,/*out*/CString &srValueType)
{
	if (m_pThread != NULL)
	{
		srVakue = m_pThread->FindVarValue(szVarFlag);
		return TRUE;
	}
	return FALSE;
}

BOOL CSinumerikDriverSer::GetVarData(/*in*/const int nVarIndex,/*out*/CString &srVakue,/*out*/CString &srValueType)
{
	if (m_pThread != NULL)
	{
		srVakue = m_pThread->FindVarValue(nVarIndex);
		return TRUE;
	}
	return FALSE;
}

BOOL CSinumerikDriverSer::GetVarMapData(CVarMapData* pData)
{
	if (m_pThread!=NULL)
	{
		m_pThread->GetVarMapData(pData);
		return TRUE;
	}	
	return FALSE;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::Start(const CHAR* szMachIP, int nPort)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("������ʼ����"));
	pThis->StartMonitor(szMachIP, nPort);
	return TRUE;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::Stop()
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("�������н���"));
	pThis->StopMonitor();
	return FALSE;
}

STDMETHODIMP_(CString) CSinumerikDriverSer::XNetMach1::GetSingleVarValue(const CHAR* sVarFlag)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("��ȡ��ǩֵ"));
	CString sValue, sType;
	pThis->GetVarData(sVarFlag, sValue, sType);
	return sValue;
}

STDMETHODIMP_(CString) CSinumerikDriverSer::XNetMach1::GetSingleVarValue(const int nIndex)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("��ȡ��ǩֵ"));
	CString sValue, sType;
	pThis->GetVarData(nIndex, sValue, sType);
	return sValue;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::GetVarMapData(CVarMapData* pData)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("��ȡ���б�ǩֵ"));
	pThis->GetVarMapData(pData);
	return FALSE;
}


