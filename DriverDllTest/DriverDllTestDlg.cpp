
// DriverDllTestDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDriverDllTestDlg �Ի���



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


// CDriverDllTestDlg ��Ϣ�������

BOOL CDriverDllTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	Init();
	m_dwCookie = 0;					// ��ʼ��cookie����ʾ��û�е��� Advise() ����
	m_Testcallbcak.SetResultWnd(&m_edit);	// ���߽���������������ʾ�����������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDriverDllTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDriverDllTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDriverDllTestDlg::OnBnClickedButtonSinumerik()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		CLSCTX_INPROC_SERVER,    // �Խ�������� DLL ��ʽ����
		IID_NetMach1,            // ��ȡ�� IUnknown �ӿ�ָ��
		(LPVOID *)&pNetMach);    //
	if (FAILED(hr))
	{
		//TRACE("FAILED");
	}
}

void CDriverDllTestDlg::OnBnClickedButtonStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (pNetMach != NULL)
	{
		HRESULT hr = pNetMach->Advise(&m_Testcallbcak, (long *)&m_dwCookie);
		if (SUCCEEDED(hr))
		{
			AfxMessageBox(_T("Advise ���óɹ����Ѿ���ȷ����"));
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
		else
		{
			AfxMessageBox(_T("Advise ����ʧ��"));
		}
	}
}


void CDriverDllTestDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();				// ȡ�ü��� �� ������
	int m_n1 = 100;
	int m_n2 = 3;
	pNetMach->Add(m_n1, m_n2);	// ����ȥ�ɣ���Ҳ��֪��ʲôʱ����н��
	// ����н���Ļ���CSink::raw_Fire_Result()�ͻᱻ����
}
