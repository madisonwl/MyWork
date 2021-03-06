// SinumerikDriverSer.cpp : 实现文件
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
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	m_pThread = NULL;
	m_bStopGetData = FALSE;

	for (int i = 0; i < 10; i++)		// 初始化所有的回调接口为 NULL
	{
		m_pCallBack[i] = NULL;
	}

	AfxOleLockApp();
}

CSinumerikDriverSer::~CSinumerikDriverSer()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	delete m_pThread;
	m_pThread = NULL;
	AfxOleUnlockApp();
}


void CSinumerikDriverSer::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}



BEGIN_MESSAGE_MAP(CSinumerikDriverSer, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CSinumerikDriverSer, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_ISinumerikDriverSer 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {8E729AA6-9E85-45E2-BC41-76FAE56EC135}
static const IID IID_ISinumerikDriverSer =
{ 0x8E729AA6, 0x9E85, 0x45E2, { 0xBC, 0x41, 0x76, 0xFA, 0xE5, 0x6E, 0xC1, 0x35 } };

static const IID IID_ISinuDataCallBack =
{ 0xDF58665D, 0x1302, 0x481E, { 0xB9, 0x8F, 0xCA, 0x8F, 0xE6, 0xC1, 0xC7, 0x66 } };

BEGIN_INTERFACE_MAP(CSinumerikDriverSer, CCmdTarget)
	INTERFACE_PART(CSinumerikDriverSer, IID_ISinumerikDriverSer, Dispatch)
	INTERFACE_PART(CSinumerikDriverSer, IID_ISinuDataCallBack, Dispatch)
	INTERFACE_PART(CSinumerikDriverSer, IID_NetMach1, NetMach1) // Add-wl-2016818
END_INTERFACE_MAP()

// {4CB51EDE-4B17-4ED2-9CD7-D62F02FB9E32}
IMPLEMENT_OLECREATE_FLAGS(CSinumerikDriverSer, "SinumerikDriver.SinumerikDriverSer", afxRegApartmentThreading, 0x4cb51ede, 0x4b17, 0x4ed2, 0x9c, 0xd7, 0xd6, 0x2f, 0x2, 0xfb, 0x9e, 0x32)


// CSinumerikDriverSer 消息处理程序
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
		CDebugInfor::ShowDebugInfor(Driver, _T("启动采集线程"));
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
	CDebugInfor::ShowDebugInfor(Driver, _T("获取设备数据"));
	return FALSE;
}

BOOL CSinumerikDriverSer::StopMonitor()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("关闭采集线程"));
	m_bStopGetData = TRUE;  //停止
	if (m_pThread)
	{
		m_pThread->DisConnect();
		Sleep(100);
		{
			m_pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			m_pThread->PostThreadMessage(WM_QUIT, NULL, NULL);
			CDebugInfor::ShowDebugInfor(Driver, _T("关闭采集线程成功"));
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
//* 函数名称：GetVarData
//* 函数功能：获取标签的值
//* 返回值： TRUE：成功；FALSE：失败
//* 参数说明：
//* 创建者：  dyx-wl
//* 创建日期：2016年10月13日
//* 修改日期：
//* 修改说明：
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

HRESULT CSinumerikDriverSer::Advise(ISinuDataCallBack * pCallBack, long * pdwCookie)
{
	for (int i = 0; i < 10; i++)	// 寻找一个保存该接口指针的位置
	{
		if (NULL == m_pCallBack[i])	// 找到了
		{
			m_pCallBack[i] = pCallBack;	// 保存到数组中
			m_pCallBack[i]->AddRef();	// 指针计数器 +1

			*pdwCookie = i + 1;			// cookie 就是数组下标
									    // +1 的目的是避免使用0，因为0表示无效
			return S_OK;
		}
	}
	return E_OUTOFMEMORY;	// 超过10个连接，内存不够用啦
}

HRESULT CSinumerikDriverSer::Unadvise(long dwCookie)
{
	if (NULL == m_pCallBack[dwCookie - 1])	// 参数错误，或该接口指针已经无效了
	{
		return E_INVALIDARG;
	}
	
	m_pCallBack[dwCookie - 1]->Release();	// 指针计数器 -1
	m_pCallBack[dwCookie - 1] = NULL;		// 空出该下标的数组元素

	return S_OK;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::Start(const CHAR* szMachIP, int nPort)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("驱动开始运行"));
	pThis->StartMonitor(szMachIP, nPort);
	return TRUE;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::Stop()
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("驱动运行结束"));
	pThis->StopMonitor();
	return FALSE;
}

STDMETHODIMP_(CString) CSinumerikDriverSer::XNetMach1::GetSingleVarValue(const CHAR* sVarFlag)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("获取标签值"));
	CString sValue, sType;
	pThis->GetVarData(sVarFlag, sValue, sType);
	return sValue;
}

STDMETHODIMP_(CString) CSinumerikDriverSer::XNetMach1::GetSingleVarValue(const int nIndex)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("获取标签值"));
	CString sValue, sType;
	pThis->GetVarData(nIndex, sValue, sType);
	return sValue;
}

STDMETHODIMP_(BOOL) CSinumerikDriverSer::XNetMach1::GetVarMapData(CVarMapData* pData)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	CDebugInfor::ShowDebugInfor(Driver, _T("获取所有标签值"));
	pThis->GetVarMapData(pData);
	return FALSE;
}

STDMETHODIMP_(HRESULT) CSinumerikDriverSer::XNetMach1::Advise(ISinuDataCallBack * pCallBack, long * pdwCookie)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	if (NULL == pCallBack)
	{
		return E_INVALIDARG;
	}
	return pThis->Advise(pCallBack, pdwCookie);
}

STDMETHODIMP_(HRESULT) CSinumerikDriverSer::XNetMach1::Unadvise(long dwCookie)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	if (dwCookie<1 || dwCookie>10)	// 这是谁干的呀？乱给参数
	{
		return E_INVALIDARG;
	}
	
	return pThis->Unadvise(dwCookie);
}

STDMETHODIMP_(HRESULT) CSinumerikDriverSer::XNetMach1::Add(long n1, long n2)
{
	METHOD_PROLOGUE(CSinumerikDriverSer, NetMach1);
	return pThis->Add(n1, n2);
}

HRESULT CSinumerikDriverSer::Add(long n1, long n2)
{
	long n = n1 + n2;
	for (int i = 0; i < 10; i++)
	{
		if (m_pCallBack[i])						// 如果回调接口有效
		{
			m_pCallBack[i]->Fire_Test(n);
		}
	}
	return S_FALSE;
}


STDMETHODIMP CSinumerikDriverSer::Fire_Test(LONG nSend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加实现代码
	return S_OK;
}
