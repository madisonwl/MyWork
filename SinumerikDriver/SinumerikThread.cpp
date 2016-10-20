// SinumerikThread.cpp : 实现文件
//

#include "stdafx.h"
#include "SinumerikThread.h"
#include "DebugInfor.h"
#include "NetMachDriverData.h"

const int g_nDeviceVarNum = 37;   //该驱动采集的变量数量
const int SINUMERIKOPER_RETRYNUM = 5;//重连计数，
#define WM_THREADMSG1 (WM_USER + 100)

// CSinumerikThread

IMPLEMENT_DYNCREATE(CSinumerikThread, CWinThread)
CSinumerikThread::CSinumerikThread()
{
	m_nPort = 8888;
	m_StopFlag = FALSE;
	m_bMsgBusy = FALSE;
	m_bServVersion = FALSE;
	m_bVarInfor = FALSE;
	m_bVarDefine = FALSE;
	m_bGetData = FALSE;
	m_bOffSet = FALSE;
	m_beConnected = FALSE;
	m_nCountTimes = 0;
}

CSinumerikThread::~CSinumerikThread()
{
	m_StopFlag = FALSE;
	m_VarvalueMap.clear();
}

BOOL CSinumerikThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	//ConnectServer();
	m_SinumerikSocket.m_pSimOperdata = &m_SimData;

	InitMap();
	return TRUE;
}

int CSinumerikThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSinumerikThread, CWinThread)
END_MESSAGE_MAP()


// CSinumerikThread 消息处理程序
BOOL CSinumerikThread::ConnectServer()
{
	BOOL ret = FALSE;
	CString sInfor = _T("连接");
	sInfor = /*m_IP +*/ _T("连接");
	CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
	ret = m_SinumerikSocket.ConnectServer(m_IP, m_nPort);
	if (ret == TRUE)
	{
		sInfor.Empty();
		sInfor = /*m_IP +*/ "连接成功";
		CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
		PostThreadMessage(WM_THREADMSG1, 11, 0);
		return TRUE;
	}
	sInfor.Empty();
	sInfor = /*m_IP + */"连接失败";
	CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
	return FALSE;
}

BOOL CSinumerikThread::DisConnect()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("关闭SOCKET"));
	m_SinumerikSocket.Disconnect();
	return FALSE;
}

void CSinumerikThread::SetStopFlag(BOOL bStopFlag)
{
	m_StopFlag = bStopFlag;
}

BOOL CSinumerikThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_THREADMSG1) && (!m_StopFlag))
	{
		switch (pMsg->wParam)
		{
		case 1:
			m_bServVersion = GetServVersion();
			m_beConnected = m_bServVersion;
			if (m_bServVersion == FALSE)
			{
				if (SINUMERIKOPER_RETRYNUM < m_nCountTimes++)
				{
					m_nCountTimes = 0;
					ConnectServer();
				}
			}
			break;
		case 2:
			m_bVarInfor = GetVarInfor();
			m_beConnected = m_bVarInfor;
			if (m_bVarInfor == FALSE)
			{
				if (SINUMERIKOPER_RETRYNUM < m_nCountTimes++)
				{
					m_nCountTimes = 0;
					ConnectServer();
				}
			}
			break;
		case 3:
			m_bOffSet = GetVarOffSet();
			m_beConnected = m_bOffSet;
			if (m_bOffSet == FALSE)
			{
				if (SINUMERIKOPER_RETRYNUM < m_nCountTimes++)
				{
					m_nCountTimes = 0;
					ConnectServer();
				}
			}
			break;
		case 4:
			m_bVarDefine = GetVarTypeDefine();
			m_beConnected = m_bVarDefine;
			if (m_bVarDefine == FALSE)
			{
				if (SINUMERIKOPER_RETRYNUM < m_nCountTimes++)
				{
					m_nCountTimes = 0;
					ConnectServer();
				}
			}
			break;
		case 5:
			if (m_SinumerikSocket.m_bConnected)
			{
				m_bGetData = GetMachData();
				m_beConnected = m_bGetData;
				if (m_beConnected == FALSE)
				{
					// 读数据错误写入日志
					m_SinumerikSocket.Disconnect();
					ConnectServer();
				}
				//memcpy(&m_SimData, m_SinumerikSocket.m_pSimOperdata, sizeof(m_SimData));
			}
			else
			{
				m_beConnected = FALSE;
				if (SINUMERIKOPER_RETRYNUM < m_nCountTimes++)
				{
					m_nCountTimes = 0;
					ConnectServer();
				}
			}
			break;
		case 11:
			GetNCData();
			break;
		default:
			break;
		}
	}
	return CWinThread::PreTranslateMessage(pMsg);
}

BOOL CSinumerikThread::GetNCData()
{
	if (m_StopFlag)
	{
		return FALSE;
	}
	if (!m_bMsgBusy)
	{
		if (m_bServVersion == FALSE)
		{
			//获得版本信息
			PostThreadMessage(WM_THREADMSG1, 1, 0);
		}
		else if (m_bVarInfor == FALSE)
		{
			//获得要采集的变量信息
			PostThreadMessage(WM_THREADMSG1, 2, 0);
		}
		else if (m_bOffSet == FALSE)
		{
			//获得变量定义信息
			PostThreadMessage(WM_THREADMSG1, 3, 0);
		}
		else if (m_bVarDefine == FALSE)
		{
			//获得变量定义信息
			PostThreadMessage(WM_THREADMSG1, 4, 0);
		}
		else
		{
			//获得数据
			PostThreadMessage(WM_THREADMSG1, 5, 0);
			if (m_SinumerikSocket.m_bConnected == TRUE)
			{
				CopyDataToMap();
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSinumerikThread::GetServVersion()
{
	return m_SinumerikSocket.GetServVersion();// 获得服务端版本号
}

BOOL CSinumerikThread::GetVarInfor()
{
	return m_SinumerikSocket.GetCNCVarinfor();// 获取机床变量信息
}
BOOL CSinumerikThread::GetVarTypeDefine()
{
	return m_SinumerikSocket.GetVarTypeDefine();
}

BOOL CSinumerikThread::GetVarOffSet()
{
	return m_SinumerikSocket.GetVarOffSet();
}

void CSinumerikThread::InitMap()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("初始化Map容器"));
	CString  sKey,sValue;
	sKey.Empty();
	sValue.Empty();

	sKey = _T("/Nck/Configuration/nckType");    //机床类型1001
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/Configuration/nckVersion");  //软件版本1002
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));
	
	sKey = _T("/Nck/Configuration/maxnumGlobMachAxes"); //有效轴1003
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numSpindles[u1]"); //转轴1004
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numGeoAxes[u1]");//几何轴1005
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numAuxAxes[u1]");//辅助轴1006
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/progName[u1]"); //程序名1007
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/workPNameLong[u1]");//主程序号1008
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/progStatus[u1]");  //程序状态1009
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/speedOvr[1]"); //主轴倍率1010
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/State/numAlarms");   //报警数1011
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/SequencedAlarms/textIndex[1]");//第一报警1012
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/SequencedAlarms/textIndex[2]"); //第二报警1013
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/TopPrioAlarm/textIndex[1]");//最高报警1014
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/actToolLength1[u1]");//当前刀具的长度1015
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Bag/State/opMode[u1]");           //设备状态1016
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/singleBlock[1]"); //执行代码和行号1017
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/actSpeed[u1,1]");      //主轴转速1018
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,1]");  //进给速度X1019
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,2]"); //进给速度Y1020
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,3]"); //进给速度Z1021
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[1]");//进给倍率X1022
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[2]"); //进给倍率Y1023
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[3]"); //进给倍率Z1024
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/driveLoad[u1,1]");//负载1025
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/MachineAxis/actToolBasePos[1]");//machinepos1:1026
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/MachineAxis/actToolBasePos[2]");//machinepos2:1027
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/MachineAxis/actToolBasePos[3]");//machinepos3:1028
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/MachineAxis/actToolBasePos[4]");//machinepos4:1029
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/MachineAxis/actToolBasePos[5]");//machinepos5:1030
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actToolBasePos[1]"); // Geo - pos1:1031
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actToolBasePos[2]");// Geo - pos2:1032
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actToolBasePos[3]");// Geo - pos3:1033
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actToolBasePos[4]");// Geo - pos4:1034
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actToolBasePos[5]");// Geo - pos5:1035
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/DriveHsa/State/motorTemperature[u1]");  // 轴温度:1036
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/actTNumber[u1]");            //工作刀具号1037
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));	
}

void CSinumerikThread::CopyDataToMap()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("采集到的数据存储到Map容器"));
	CString  sKey, sValue;

	sKey = _T("/Nck/Configuration/nckType");    //机床类型
	sValue.Format(_T("%ld"),m_SimData.dwnckType);
	m_VarvalueMap[sKey] = sValue; 
	sValue.Empty();

	sKey = _T("/Nck/Configuration/nckVersion");  //软件版本
	sValue.Format(_T("%.0f"), m_SimData.dbnckVersion);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	sKey = _T("/Nck/Configuration/maxnumGlobMachAxes");//机床最大有效轴数
	sValue.Format(_T("%d"), m_SimData.nmaxnumGlobMachAxes);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//转轴数量
	sKey = _T("/Channel/Configuration/numSpindles[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumSpindles);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//几何轴和方向轴数量
	sKey = _T("/Channel/Configuration/numGeoAxes[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumGeoAxes);
	m_VarvalueMap[sKey] = sValue;
	sKey.Empty();

	//辅助轴数量
	sKey = _T("/Channel/Configuration/numAuxAxes[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumAuxAxes);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//当前执行程序名
	sKey = _T("/Channel/ProgramInfo/progName[u1]");
	//sValue.Format(_T("%s"), m_SimData.cprogName);
	sValue = m_SimData.cprogName;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//主程序号
	sKey = _T("/Channel/ProgramInfo/workPNameLong[u1]");
	//sValue.Format(_T("%s"), m_SimData.cworkPNameLong);
	sValue = m_SimData.cworkPNameLong;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//程序状态
	sKey = _T("/Channel/State/progStatus[u1]");
	switch (m_SimData.nprogStatus){
	case 1:
		sValue = _T("interrupted");
		break;
	case 2:
		sValue = _T("stopped");
		break;
	case 3:
		sValue = _T("in progress");
		break;
	case 4:
		sValue = _T("waiting");
		break;
	case 5:
		sValue = _T("aborted");
		break;
	default:
		break;
	}
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//主轴倍率
	sKey = _T("/Channel/Spindle/speedOvr[1]");
	sValue.Format(_T("%d"), m_SimData.nspeedOvr);
	m_VarvalueMap[sKey] = sValue;

	//报警数量
	sKey = _T("/Nck/State/numAlarms");
	sValue.Format(_T("%d"), m_SimData.nnumAlarms);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//第一报警号
	sKey = _T("/Nck/SequencedAlarms/textIndex[1]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex1);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
	//第二报警号
	sKey = _T("/Nck/SequencedAlarms/textIndex[2]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex2);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//最高报警号
	sKey = _T("/Nck/TopPrioAlarm/textIndex[1]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex11);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//刀具长度
	sKey = _T("/Channel/State/actToolLength1[u1]");

	//设备状态
	sKey = _T("/Bag/State/opMode[u1]");
	switch (m_SimData.nopMode) 
	{
	case 0:
		sValue = _T("JOG");
		break;
	case 1:
		sValue = _T("MDA");
		break;
	case 2:
		sValue = _T("AUTO");
		break;
	default:
		break;
	}
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
	
	//执行代码
	sKey = _T("/Channel/ProgramInfo/singleBlock[1]");
	//sValue.Format(_T("%s"), m_SimData.csingleBlock);
	sValue = m_SimData.csingleBlock;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//主轴转速
	sKey = _T("/Channel/Spindle/actSpeed[u1,1]");
	sValue.Format(_T("%d"), m_SimData.dbactSpeed);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//进给速度
	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,1]");
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));
	sValue.Format(_T("%d"), fabs(m_SimData.dbactFeedRate1));
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

/*
	sKey = "/Channel/GeometricAxis/actFeedRate[u1,2]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actFeedRate[u1,3]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));*/

	//进给倍率
	sKey = _T("/Channel/GeometricAxis/feedRateOvr[1]");
	sValue.Format(_T("%d"), m_SimData.dbfeedRateOvr1);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
	
/*
	sKey = "/Channel/GeometricAxis/feedRateOvr[2]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/feedRateOvr[3]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));*/

	//主轴负荷
	sKey = _T("/Channel/Spindle/driveLoad[u1,1]");
	sValue.Format(_T("%d"), m_SimData.dbdriveLoad);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
/*
	sKey = "/Channel/MachineAxis/actToolBasePos[1]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/MachineAxis/actToolBasePos[2]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/MachineAxis/actToolBasePos[3]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/MachineAxis/actToolBasePos[4]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/MachineAxis/actToolBasePos[5]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actToolBasePos[1]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actToolBasePos[2]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actToolBasePos[3]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actToolBasePos[4]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/actToolBasePos[5]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/DriveHsa/State/motorTemperature[u1]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));*/

	sKey = _T("/Channel/State/actTNumber[u1]");
	sValue.Format(_T("T%d"), m_SimData.ltoolno);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
}

BOOL CSinumerikThread::GetMachData()
{
	return m_SinumerikSocket.GetNCData();
}

CString CSinumerikThread::FindVarValue(const CHAR* sVarFlag)
{
	CString sKey(sVarFlag);
	CString sValue = m_VarvalueMap[sKey];
	return sValue;
}

CString CSinumerikThread::FindVarValue(const int nVarIdex)
{
	CString sValue(_T("本驱动不支持该索引方法"));
	return sValue;
}

BOOL CSinumerikThread::GetVarMapData(CVarMapData* pData)
{
	pData->m_VarvalueMap = m_VarvalueMap;
	return TRUE;
}
