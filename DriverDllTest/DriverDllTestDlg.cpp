
// DriverDllTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DriverDllTest.h"
#include "DriverDllTestDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "NetMachDriverData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDriverDllTestDlg 对话框



CDriverDllTestDlg::CDriverDllTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDriverDllTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pNetMach = NULL;
	pData = new CVarMapData;
}

void CDriverDllTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CDriverDllTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SINUMERIK, &CDriverDllTestDlg::OnBnClickedButtonSinumerik)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDriverDllTestDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDOK, &CDriverDllTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDriverDllTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GETALL, &CDriverDllTestDlg::OnBnClickedButtonGetall)
	ON_BN_CLICKED(IDC_BUTTON2, &CDriverDllTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDriverDllTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDriverDllTestDlg 消息处理程序

BOOL CDriverDllTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	Init();
	m_dwCookie = 0;					// 初始化cookie，表示还没有调用 Advise() 函数
	m_Testcallbcak.SetResultWnd(&m_edit);	// 告诉接收器，计算结果显示在这个窗口中
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDriverDllTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDriverDllTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDriverDllTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDriverDllTestDlg::OnBnClickedButtonSinumerik()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pNetMach != NULL)
	{
		const CHAR* pMachIP = "192.168.6.110";
		int nPort = 8888;
		pNetMach->Start(pMachIP, nPort);
	}
	
}

void CDriverDllTestDlg::Init()
{
	CoInitialize(NULL);
	HRESULT hr;
	CString str = _T("SinumerikDriver.SinumerikDriverSer");
	USES_CONVERSION; 
	LPCOLESTR strOPCServName = /*A2W*/(str);
	//str.ReleaseBuffer();
	CLSID clsid;
	hr = CLSIDFromProgID(strOPCServName, &clsid);
	if (FAILED(hr))
	{
	//	TRACE("FAILED");
	}
	hr = CoCreateInstance(clsid, NULL,
		CLSCTX_INPROC_SERVER,    // 以进程内组件 DLL 方式加载
		IID_NetMach1,            // 想取得 IUnknown 接口指针
		(LPVOID *)&pNetMach);    //
	if (FAILED(hr))
	{
		//TRACE("FAILED");
	}
}

void CDriverDllTestDlg::OnBnClickedButtonStop()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pNetMach != NULL)
	{
		pNetMach->Stop();
		pNetMach->Release();
		pNetMach = NULL;
	}
	CoUninitialize();
}


void CDriverDllTestDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pNetMach != NULL)
	{
		pNetMach->Stop();
		pNetMach->Release();
		pNetMach = NULL;
	}
	delete pData;
	pData = NULL;
	CDialogEx::OnOK();
}


void CDriverDllTestDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	if(pNetMach != NULL)
	{
		const CHAR* pVarFlag = "/Nck/Configuration/nckType";
		CString sValue;
		sValue = pNetMach->GetSingleVarValue(pVarFlag);
		AfxMessageBox(sValue);
	}
}


void CDriverDllTestDlg::OnBnClickedButtonGetall()
{
	// TODO:  在此添加控件通知处理程序代码
	
	if (pNetMach != NULL)
	{
		const CHAR* pVarFlag = "/Nck/Configuration/nckType";
		CString sValue;
		pNetMach->GetVarMapData(pData);
		AfxMessageBox(sValue);
	}
}


void CDriverDllTestDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pNetMach != NULL)
	{
		HRESULT hr = pNetMach->Advise(&m_Testcallbcak, (long *)&m_dwCookie);
		if (SUCCEEDED(hr))
		{
			AfxMessageBox(_T("Advise 调用成功。已经正确连接"));
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
		else
		{
			AfxMessageBox(_T("Advise 调用失败"));
		}
	}
}


void CDriverDllTestDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();				// 取得加数 和 被加数
	int m_n1 = 100;
	int m_n2 = 3;
	pNetMach->Add(m_n1, m_n2);	// 计算去吧，我也不知道什么时候会有结果
	// 如果有结果的话，CSink::raw_Fire_Result()就会被调用
}
