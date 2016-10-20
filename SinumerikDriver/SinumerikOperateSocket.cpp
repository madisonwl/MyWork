// SinumerikOperateSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SinumerikOperateSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSinumerikOperateSocket
const CString SINUMERIK_VSERSION = _T("SINUMERIK_VSERSION");    //840DSL服务端版本号
const CString SINUMERIK_VARFLAG = _T("SINUMERIK_VARFLAG");      //840DSL变量标签号，每个变量对应一个标签号
const CString SINUMERIK_VARDEFINE = _T("SINUMERIK_VARDEFINE");  //变量类型定义
const CString SINUMERIK_VAROFFSET = _T("SINUMERIK_VAROFFSET");  //解析偏移大小消息
const CString SINUMERIK_VARDATA = _T("SINUMERIK_VARDATA");      //获得变量值
const CString SINUMERIK_BASICINFOR = _T("SINUMERIK_BASICINFOR");//获得机床基本信息

const UINT SIM_HEADER_SIZE = 16;  //信息头文件长度，数据信息大于头长度，如果小于头长度，则认为是版本号
const UINT SIM_BUF_SIZE = 2048;	  // 840DSL数据缓冲区大小
const UINT SIM_MAXVAR_SIZE = 256; //最长的char变量

const CString SIM_HEADER_MSG = _T("version:");

const UINT SINUMERIKVARFLAG = 1;
const UINT SINUMERIKVAROFFSET = 2;
const UINT SINUMERIKVARDEFINE = 3;
const UINT SINUMERIKVARDATA = 4;
const UINT SINUMERIKVARALARM = 9;
CSinumerikOperateSocket::CSinumerikOperateSocket()
{
	m_bConnected = FALSE;
	m_bConnected = FALSE;
	m_pcBuffer = new char[SIM_BUF_SIZE];
	memset(m_pcBuffer, 0x00, SIM_BUF_SIZE);

	tempHeadbuff = new char[SIM_HEADER_SIZE];
	memset(tempHeadbuff, 0x00, SIM_HEADER_SIZE);

	tempDatabuff = new char[SIM_BUF_SIZE];
	memset(tempDatabuff, 0x00, SIM_BUF_SIZE);

	tempvarbuff = new char[SIM_MAXVAR_SIZE];
	memset(tempvarbuff, 0x00, SIM_MAXVAR_SIZE);

	m_pSimOperdata = NULL;
	m_bGetValue = TRUE;

	m_pSimVarData = new SimVarData;
	m_pSimVarData->dbVarVal = 0;
	m_pSimVarData->dwVarVal = 0;
	m_pSimVarData->lVarVal = 0;
	m_pSimVarData->nVarVal = 0;
	

}

CSinumerikOperateSocket::~CSinumerikOperateSocket()
{
	delete[] m_pcBuffer;
	m_pcBuffer = NULL;

	delete[] tempDatabuff;
	tempDatabuff = NULL;

	delete[] tempHeadbuff;
	tempHeadbuff = NULL;

	delete[] tempvarbuff;
	tempvarbuff = NULL;

	if (m_pSimVarData != NULL)
	{
		delete m_pSimVarData;
		m_pSimVarData=NULL;
	}
	delete m_pSimVarData;
	m_pSimOperdata = NULL;
	FreeList();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSinumerikOperateSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CSinumerikOperateSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0
BOOL CSinumerikOperateSocket::ConnectServer( const CString& sServerIP, const int nPort )
{
	if (("0.0.0.0" == sServerIP) || sServerIP.IsEmpty())
	{
		return FALSE;
	}
	ShutDown();
	Close();
	try
	{
		AfxSocketInit();
		if (Create())
		{	
			if (!AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT))
			{
				ShutDown();
				Close();
				//GetSocketErrorText();			
				return FALSE;
			}	
			Connect(sServerIP, nPort);
		    //FreeList();
			m_bConnected = TRUE;
			return TRUE;
		}
	}
	catch (...)
	{
		//		Close();
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CSinumerikOperateSocket member functions

void CSinumerikOperateSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		if (0 == nErrorCode)
		{
			memset(m_pcBuffer, 0x00, SIM_BUF_SIZE);
			int nBytes = Receive(m_pcBuffer, SIM_BUF_SIZE);
			switch (nBytes)
			{
			case 0:// connection has been closed
				ShutDown();//释放Socket资源占用问题
				Close();
				m_bConnected = FALSE;
				break;
			case SOCKET_ERROR:// read error
				if(GetLastError()!=WSAEWOULDBLOCK)
				{
					m_bConnected = FALSE;
				//	TRACE("OnReceive errror: %d\n", GetLastError());
				}
				break;
			default:
				ProcessMsg(nBytes);
				break;
			}
		}
		else if (WSAENETDOWN == nErrorCode)
		{
		}
	}
	catch (...)
	{
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSinumerikOperateSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		ShutDown();
		Close();
	}
	catch (...)
	{
	}
	m_bConnected = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}

void CSinumerikOperateSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (0 == nErrorCode)
	{
	//	m_bConnected = TRUE;
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

BOOL CSinumerikOperateSocket::AnalyseProductinfor(const char* buff)
{
	int nTempLength =0;
	UINT nTemp = 0;
	int nTemp1 = 0;
	int iii=0;
	char cTemp[128];
	memset(cTemp,0,128);
	CString strTemp;
	CString strTemp1;
	CString strFlag;
	int nFlag = -1;
	memcpy(cTemp,buff,3);
	strTemp.Format(_T("%s"),cTemp,3);
	if (strTemp=="DNC")  //判断是否是DNC信息
	{
		memset(cTemp,0,128);
		strTemp.Empty();
		strTemp.Format(_T("%s"),buff+3);
		int nLength = strTemp.GetLength();
		if (nLength<=2)  //标识符长度为2
		{
			return FALSE;
		}
		else
		{
			strTemp1 = strTemp.Mid(0,2);
			if (strTemp1 == "01")  //标识位正确
			{
				strTemp1 = strTemp.Mid(2,nLength-1);
				if (strTemp1.GetLength()>128)
				{
					return FALSE;
				}
				USES_CONVERSION;
				const char* pTemp = T2A(strTemp1);
				strncpy_s(m_pSimOperdata->cWorkOrder, pTemp, strTemp1.GetLength());
				return TRUE;
			}
		}	
	}
	return FALSE;
}
void CSinumerikOperateSocket::FreeList()
{
	POSITION pos=NULL;
	pos = SimVarParaList.GetHeadPosition();
	SimVarPara *pvarpara1 = NULL;
	if(pos)
	{
		while(pos)
		{
			pvarpara1=SimVarParaList.GetNext(pos);
			if(pvarpara1)
			{
				delete pvarpara1;
				pvarpara1=NULL;
			}
		}
		SimVarParaList.RemoveAll();
	}
}

void CSinumerikOperateSocket::Disconnect()
{
	if (m_bConnected)
	{
		try
		{
			ShutDown();
			Close();
		}
		catch (...)
		{
		}
		m_bConnected = FALSE;
	}	
}
//********************************************************************
//* 函数名称：ProcessMsg
//* 函数功能：OnReceive接收到的消息处理
//* 返回值：  
//* 参数说明：
//* 创建者：  dyx-wl
//* 创建日期：2016年10月09日
//* 修改日期：
//* 修改说明：
//********************************************************************
void CSinumerikOperateSocket::ProcessMsg(const int nByte)
{
	if (nByte == SIM_HEADER_SIZE)//接收到的消息小于数据消息头，接收的为服务端版本信息，格式为version:1.0  CString+double格式
	{
		//	char* tempbuff = new char[SIM_HEADER_SIZE];
		memcpy(tempHeadbuff, m_pcBuffer, SIM_HEADER_SIZE);
		AnalyseMsgVersion(tempHeadbuff);
	}
	else if (nByte > SIM_HEADER_SIZE) //接收到的消息大于数据消息头，说明消息头后面带有数据,格式:版本号(double) + 消息类型(int) + 数据个数(int) + 数据
	{
		memcpy(tempDatabuff, m_pcBuffer, nByte);
		AnalyseMsgData(tempDatabuff);
	}
}

//********************************************************************
//* 函数名称：AnalyseMsgVersion
//* 函数功能：解析OnReceive接收到的消息处理，得到版本号
//* 返回值：  无
//* 参数说明：buff：收到的字符串;dbver:解析得到的版本号
//* 创建者：  dyx-wl
//* 创建日期：2016年10月09日
//* 修改日期：
//* 修改说明：
//********************************************************************
void CSinumerikOperateSocket::AnalyseMsgVersion(const char* buff)
{
	char cTemp[9];
	memset(cTemp, 0, 9);
	memcpy(cTemp, buff, 8);
	CString strTemp;
	strTemp.Format(_T("%s"), buff);
	if (strTemp == SIM_HEADER_MSG)
	{
		m_dbSerVersion = *(double*)(buff + 8);
		m_strSerVersion.Format(_T("%.1f"), m_dbSerVersion);
	}
}

void CSinumerikOperateSocket::AnalyseMsgData(const char* buff)
{
	//解析得到消息类型
	int nMsgType = 0;
	nMsgType = *(UINT*)(buff + 8);
	switch (nMsgType)
	{
	case SINUMERIKVARFLAG:
		SaveVarFlag(buff);
		break;
	case SINUMERIKVAROFFSET:
		SaveVarOffSet(buff);
		break;
	case SINUMERIKVARDEFINE:
		SaveVarDefine(buff);
		break;
	case SINUMERIKVARDATA:
		SaveVarValue(buff);
		break;
	default:
		break;
	}
}

void CSinumerikOperateSocket::SaveVarFlag(const char* buff)
{
	int nVarNum = 0;
	nVarNum = *(UINT*)(buff + sizeof(double)+sizeof(int));
	char cTemp[5];
	memset(cTemp, 0, 5);
	//CString strVarTemp;
	for (int i = 0; i < nVarNum; i++)
	{
		SimVarPara *pvarpara = new SimVarPara;
		memcpy(cTemp, buff + sizeof(double)+sizeof(int)* 2 + i * 4, 4);
		//strVarTemp.Format(_T("%s"), cTemp, 4);
		CString strVarTemp(cTemp);
		pvarpara->VarFlag = strVarTemp;
		SimVarParaList.AddTail(pvarpara);
	}
}

void CSinumerikOperateSocket::SaveVarOffSet(const char* buff)
{
	int nVarNum = 0;
	char cTemp[5];
	memset(cTemp, 0, 5);
	CString strVarTemp;

	POSITION pos = NULL;
	pos = SimVarParaList.GetHeadPosition();
	SimVarPara *pvarpara1 = NULL;
	m_nAlloffSet = 0;
	while (pos)
	{
		pvarpara1 = SimVarParaList.GetAt(pos);
		pvarpara1->nVarOffSet = *(UINT*)(buff + sizeof(double)+sizeof(int)* 2 + nVarNum * 4);
		m_nAlloffSet += pvarpara1->nVarOffSet;
		SimVarParaList.SetAt(pos, pvarpara1);
		SimVarParaList.GetNext(pos);
		nVarNum++;
	}
	int a = 0;
}

void CSinumerikOperateSocket::SaveVarDefine(const char* buff)
{
	int nVarNum = 0;
	char cTemp[3];
	memset(cTemp, 0, 3);
	CString strVarTemp;

	POSITION pos = NULL;
	pos = SimVarParaList.GetHeadPosition();
	SimVarPara *pvarpara1 = NULL;
	while (pos)
	{
		pvarpara1 = SimVarParaList.GetAt(pos);
		memcpy(cTemp, buff + sizeof(double)+sizeof(int)* 2 + nVarNum, 1);
		//pvarpara1->VarType.Format(_T("%s"), cTemp);
		pvarpara1->VarType = cTemp;
		SimVarParaList.SetAt(pos, pvarpara1);
		SimVarParaList.GetNext(pos);
		nVarNum++;
	}
}

void CSinumerikOperateSocket::SaveVarValue(const char* buff)
{
	m_bGetValue = GetAllVarValue(SimVarParaList, buff);
}

BOOL CSinumerikOperateSocket::GetAllVarValue(const CTypedPtrList<CPtrList, SimVarPara*> &SimVarParaList, const char* buff)
{
	int nVarNum = 0;
	POSITION pos = NULL;
	pos = SimVarParaList.GetHeadPosition();
	if (pos == NULL)
	{
		return FALSE;
	}
	int nAllOffSet = 0;   //总的偏移量
	int nOffSet = 0;
	CString strVarType;   //类型
	CString strVarFlag;

	CString strbuff;  //测试用
	int nFlag;
	CString sAlarmTem;

	SimVarPara *pvarpara1 = NULL;
	while (pos)
	{
		memset(tempvarbuff, 0x00, SIM_MAXVAR_SIZE);
		pvarpara1 = SimVarParaList.GetAt(pos);
		nOffSet = pvarpara1->nVarOffSet; //偏移量
		if (nOffSet > SIM_MAXVAR_SIZE || nOffSet < 0)   //如果解析的内存偏移量大于
		{
			return FALSE;
		}
		strVarType = pvarpara1->VarType;  //1:DWORD;2:int;3:double 4:char; 5:long
		if (strVarType == "1")
		{
			m_pSimVarData->dwVarVal = *(DWORD*)(buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet);
			nAllOffSet += nOffSet;
		}
		else if (strVarType == "2")
		{
			m_pSimVarData->nVarVal = *(UINT*)(buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet);
			nAllOffSet += nOffSet;
		}
		else if (strVarType == "3")
		{
			m_pSimVarData->dbVarVal = *(double*)(buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet);
			nAllOffSet += nOffSet;
		}
		else if (strVarType == "4")
		{
			if (nOffSet < 0)
			{
				return FALSE;
			}
			//读取字符串的长度
			int nlength = 0;
			nlength = *(UINT*)(buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet);
			nAllOffSet += sizeof(int);

			memcpy(tempvarbuff, buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet, nlength);
			nAllOffSet += nlength;
		}
		else if (strVarType == "5")
		{
			m_pSimVarData->lVarVal = *(long*)(buff + sizeof(double)+sizeof(int)* 2 + nAllOffSet);
			nAllOffSet += nOffSet;
		}
		else
		{
			return FALSE;
		}
		strVarFlag = pvarpara1->VarFlag;
		nFlag = _wtoi(strVarFlag);
		
		strbuff = _T("DNC01Test0907");
		USES_CONVERSION;
		const char* pMsg = T2A(strbuff);
		//	if (nFlag>9999 || nFlag<1001)
		if (nFlag > 10000 || nFlag < 1001 || nOffSet < 0)
		{
			return FALSE;
		}
		switch (nFlag)
		{
		case 1001:
			m_pSimOperdata->dwnckType = m_pSimVarData->dwVarVal;
			break;
		case 1002:
			m_pSimOperdata->dbnckVersion = m_pSimVarData->dbVarVal;
			break;
		case 1003:
			m_pSimOperdata->nmaxnumGlobMachAxes = m_pSimVarData->nVarVal;
			break;
		case 1004:
			m_pSimOperdata->nnumSpindles = m_pSimVarData->nVarVal;
			break;
		case 1005:
			m_pSimOperdata->nnumGeoAxes = m_pSimVarData->nVarVal;
			break;
		case 1006:
			m_pSimOperdata->nnumAuxAxes = m_pSimVarData->nVarVal;
			break;
		case 1007:
			memcpy(m_pSimOperdata->cprogName, tempvarbuff, nOffSet);
			break;
		case 1008:
			memcpy(m_pSimOperdata->cworkPNameLong, tempvarbuff, nOffSet);
			break;
		case 1009:
			m_pSimOperdata->nprogStatus = m_pSimVarData->nVarVal;
			break;
		case 1010:
			m_pSimOperdata->nspeedOvr = m_pSimVarData->nVarVal;
			break;
		case 1011:
			m_pSimOperdata->nnumAlarms = m_pSimVarData->nVarVal;
			break;
		case 1012:
			m_pSimOperdata->ltextIndex1 = m_pSimVarData->lVarVal;
			break;
		case 1013:
			m_pSimOperdata->ltextIndex2 = m_pSimVarData->lVarVal;
			break;
		case 1014:
			m_pSimOperdata->ltextIndex11 = m_pSimVarData->lVarVal;
			break;
		case 1015:
			m_pSimOperdata->dbactToolLength1 = m_pSimVarData->dbVarVal;
			break;
		case 1016:
			m_pSimOperdata->nopMode = m_pSimVarData->nVarVal;
			break;
		case 1017:
			memcpy(m_pSimOperdata->csingleBlock, tempvarbuff, nOffSet);
			break;
		case 1018:
			m_pSimOperdata->dbactSpeed = m_pSimVarData->dbVarVal;
			break;
		case 1019:
			m_pSimOperdata->dbactFeedRate1 = m_pSimVarData->dbVarVal;
			break;
		case 1020:
			m_pSimOperdata->dbactFeedRate2 = m_pSimVarData->dbVarVal;
			break;
		case 1021:
			m_pSimOperdata->dbactFeedRate3 = m_pSimVarData->dbVarVal;
			break;
		case 1022:
			m_pSimOperdata->dbfeedRateOvr1 = m_pSimVarData->dbVarVal;
			break;
		case 1023:
			m_pSimOperdata->dbfeedRateOvr2 = m_pSimVarData->dbVarVal;
			break;
		case 1024:
			m_pSimOperdata->dbfeedRateOvr3 = m_pSimVarData->dbVarVal;
			break;
		case 1025:
			m_pSimOperdata->dbdriveLoad = m_pSimVarData->dbVarVal;
			break;
		case 1026:
			m_pSimOperdata->dbmaactToolBasePos1 = m_pSimVarData->dbVarVal;
			break;
		case 1027:
			m_pSimOperdata->dbmaactToolBasePos2 = m_pSimVarData->dbVarVal;
			break;
		case 1028:
			m_pSimOperdata->dbmaactToolBasePos3 = m_pSimVarData->dbVarVal;
			break;
		case 1029:
			m_pSimOperdata->dbmaactToolBasePos4 = m_pSimVarData->dbVarVal;
			break;
		case 1030:
			m_pSimOperdata->dbmaactToolBasePos5 = m_pSimVarData->dbVarVal;
			break;
		case 1031:
			m_pSimOperdata->dbgeactToolBasePos1 = m_pSimVarData->dbVarVal;
			break;
		case 1032:
			m_pSimOperdata->dbgeactToolBasePos2 = m_pSimVarData->dbVarVal;
			break;
		case 1033:
			m_pSimOperdata->dbgeactToolBasePos3 = m_pSimVarData->dbVarVal;
			break;
		case 1034:
			m_pSimOperdata->dbgeactToolBasePos4 = m_pSimVarData->dbVarVal;
			break;
		case 1035:
			m_pSimOperdata->dbgeactToolBasePos5 = m_pSimVarData->dbVarVal;
			break;
		case 1036:
			m_pSimOperdata->nmotorTemperature = m_pSimVarData->nVarVal;
			break;
		case 1037:
			m_pSimOperdata->ltoolno = m_pSimVarData->lVarVal;
			break;
		case 1038:   //随动误差
			m_pSimOperdata->dbAxisFollowErr[0] = m_pSimVarData->dbVarVal;
			break;
		case 1039:
			m_pSimOperdata->dbAxisFollowErr[1] = m_pSimVarData->dbVarVal;
			break;
		case 1040:
			m_pSimOperdata->dbAxisFollowErr[2] = m_pSimVarData->dbVarVal;
			break;
		case 1041:
			m_pSimOperdata->dbAxisFollowErr[3] = m_pSimVarData->dbVarVal;
			break;
		case 1042:
			m_pSimOperdata->dbAxisFollowErr[4] = m_pSimVarData->dbVarVal;
			break;
		case 1043:   //轴温度
			m_pSimOperdata->nAxisTemperature[0] = m_pSimVarData->nVarVal;
			break;
		case 1044:
			m_pSimOperdata->nAxisTemperature[1] = m_pSimVarData->nVarVal;
			break;
		case 1045:
			m_pSimOperdata->nAxisTemperature[2] = m_pSimVarData->nVarVal;
			break;
		case 1046:
			m_pSimOperdata->nAxisTemperature[3] = m_pSimVarData->nVarVal;
			break;
		case 1047:
			m_pSimOperdata->nAxisTemperature[4] = m_pSimVarData->nVarVal;
			break;
		case 1048:   //轴电流
			m_pSimOperdata->nAxisCurrent[0] = m_pSimVarData->nVarVal;
			break;
		case 1049:
			m_pSimOperdata->nAxisCurrent[1] = m_pSimVarData->nVarVal;
			break;
		case 1050:
			m_pSimOperdata->nAxisCurrent[2] = m_pSimVarData->nVarVal;
			break;
		case 1051:
			m_pSimOperdata->nAxisCurrent[3] = m_pSimVarData->nVarVal;
			break;
		case 1052:
			m_pSimOperdata->nAxisCurrent[4] = m_pSimVarData->nVarVal;
			break;
		case 9998:  //工单号
			memcpy(m_pSimOperdata->cProductinfor, tempvarbuff, nOffSet);
#ifdef _DEBUG
			
			strncpy_s(m_pSimOperdata->cProductinfor, pMsg, strbuff.GetLength());
#endif
			if (AnalyseProductinfor(m_pSimOperdata->cProductinfor))  //解析自定义的信息
			{
				strncpy_s(m_pSimOperdata->cProductinfor, m_pSimOperdata->cWorkOrder, 128);  //解析得到工单号
			}
			break;
		case 9999:  //待机原因
			memcpy(m_pSimOperdata->cStopInforData, tempvarbuff, nOffSet);
			break;
		case 9000: //第一报警
			memcpy(m_pSimOperdata->cFirstAlm, tempvarbuff, nOffSet);
			break;
		case 9001: //第二报警
			memcpy(m_pSimOperdata->cSecondAlm, tempvarbuff, nOffSet);
			break;
		case 9002: //最高报警
			memcpy(m_pSimOperdata->cTopAlm, tempvarbuff, nOffSet);
			break;
		default:
			break;
		}
		SimVarParaList.GetNext(pos);
	}
	sAlarmTem = ComPoundAlarm();
	memset(m_pSimOperdata->cAlarmMsg, 0, sizeof(m_pSimOperdata->cAlarmMsg));
	memcpy(m_pSimOperdata->cAlarmMsg, sAlarmTem, sAlarmTem.GetLength());   //合成报警信息
	return TRUE;
}

CString CSinumerikOperateSocket::ComPoundAlarm()
{
	CString sMsgTmp;
	CString sMsg;
	CString sFirstAlmMsgTmp, sSecondAlmMsgTmp, sTopAlmMsgTmp;

	sFirstAlmMsgTmp = m_pSimOperdata->cFirstAlm;
	sSecondAlmMsgTmp = m_pSimOperdata->cSecondAlm;
	sTopAlmMsgTmp = m_pSimOperdata->cTopAlm;

	if (m_pSimOperdata->ltextIndex1 != 0)  //第一报警报警号
	{
		sMsgTmp.Format(_T("%ld-%s;"), m_pSimOperdata->ltextIndex1, m_pSimOperdata->cFirstAlm);
		sMsg = sMsgTmp;
		sMsgTmp.Empty();
	}
	if (m_pSimOperdata->ltextIndex2 != 0 && sFirstAlmMsgTmp != sSecondAlmMsgTmp)  //第二报警号
	{
		sMsgTmp.Format(_T("%ld-%s;"), m_pSimOperdata->ltextIndex2, m_pSimOperdata->cSecondAlm);
		sMsg += sMsgTmp;
		sMsgTmp.Empty();
	}

	if (sTopAlmMsgTmp != sFirstAlmMsgTmp && sTopAlmMsgTmp != sSecondAlmMsgTmp)
	{
		sMsgTmp.Format(_T("%ld-%s;"), m_pSimOperdata->ltextIndex11, m_pSimOperdata->cTopAlm);
		sMsg += sMsgTmp;
		sMsgTmp.Empty();
	}
	return sMsg;
}


int CSinumerikOperateSocket::GetSocketErrorText()
{
	int nError = GetLastError();
	switch (nError)
	{
	case WSAENOTSOCK://10038
		TRACE("Socket error: Socket operation on non-socket\n");
		break;
	case WSAEADDRINUSE://10048
		TRACE("Socket error: Address already in ues\n");
		break;
	case WSAENETUNREACH://10051
		TRACE("Socket error: Network is unreachable\n");
		break;
	case WSAENETRESET://10052
		TRACE("Socket error: Network dropped connection on reset\n");
		break;
	case WSAECONNABORTED://10053
		TRACE("Socket error: Software caused connection abort\n");
		break;
	case WSAECONNRESET://10054
		TRACE("Socket error: Connection reset by peer\n");
		//Close();
		break;
	case WSAEISCONN://10056
		TRACE("Socket error: Socket is already connected\n");
		break;
	case WSAENOTCONN://10057
		TRACE("Socket error: Socket is not connected\n");
		break;
	case WSAESHUTDOWN://10058
		TRACE("Socket error: Cannot send after socket shutdown\n");
		break;
	case WSAETIMEDOUT://10060
		TRACE("Socket error: Connection timed out\n");
		break;
	case WSAECONNREFUSED://10061
		TRACE("Socket error: Connection refused\n");
		break;
	default:
		TRACE("Socket error: Other fault %d\n", nError);
		break;
	}
	return nError;
}

BOOL CSinumerikOperateSocket::SendReadCmd(const CString strCmd)
{
	if (strCmd.IsEmpty())
	{
		return FALSE;
	}
	try
	{
		//int nBytes = Send(strCmd, strlen(strCmd));
		int nLength = strCmd.GetLength();
		USES_CONVERSION;
		const char* pBuff = T2A(strCmd);
		//合成报警信息
		int nBytes = Send(pBuff, nLength);
		switch (nBytes)
		{
		case SOCKET_ERROR:
			if (WSAEWOULDBLOCK == GetLastError())
			{
				GetSocketErrorText();
				m_bConnected = FALSE;
			}
			else
			{
				return FALSE;
			}
			break;
		default:
			return TRUE;
			break;
		}
	}
	catch (...)
	{
	}

	return FALSE;
}

BOOL CSinumerikOperateSocket::GetServVersion()
{
	if (SendReadCmd(SINUMERIK_VSERSION))   //发送读取版本指令
	{
		TRACE("Socket-GetServVersion成功\n");
		return TRUE;
	}
	else
	{
		TRACE("Socket-GetServVersion失败\n");
		return FALSE;
	}
	return FALSE;
}

BOOL CSinumerikOperateSocket::GetCNCVarinfor()
{
	if (SendReadCmd(SINUMERIK_VARFLAG))
	{
		TRACE("Socket-GetCNCVarinfor成功\n");
		Sleep(8);
		return TRUE;
	}
	else
	{
		TRACE("Socket-GetCNCVarinfor失败\n");
		return FALSE;
	}
	return FALSE;
}

BOOL CSinumerikOperateSocket::GetVarTypeDefine()
{
	if (SendReadCmd(SINUMERIK_VARDEFINE))
	{
		TRACE("Socket-GetVarTypeDefine成功\n");
		Sleep(8);
		return TRUE;
	}
	else
	{
		TRACE("Socket-GetVarTypeDefine失败\n");
		return FALSE;
	}
	return FALSE;
}

BOOL CSinumerikOperateSocket::GetVarOffSet()
{
	if (SendReadCmd(SINUMERIK_VAROFFSET))
	{
		TRACE("Socket-GetVarOffSet成功\n");
		Sleep(8);
		return TRUE;
	}
	else
	{
		TRACE("Socket-GetVarOffSet失败\n");
		return FALSE;
	}
	return FALSE;
}

BOOL CSinumerikOperateSocket::GetNCData()
{
	if (SendReadCmd(SINUMERIK_VARDATA))
	{
		TRACE("Socket-GetNCData成功\n");
		Sleep(8);
		return TRUE;
	}
	else
	{
		TRACE("Socket-GetNCData失败\n");
		return FALSE;
	}
	return FALSE;
}
