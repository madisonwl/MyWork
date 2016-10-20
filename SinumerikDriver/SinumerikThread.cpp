// SinumerikThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SinumerikThread.h"
#include "DebugInfor.h"
#include "NetMachDriverData.h"

const int g_nDeviceVarNum = 37;   //�������ɼ��ı�������
const int SINUMERIKOPER_RETRYNUM = 5;//����������
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
	// TODO:    �ڴ�ִ���������̳߳�ʼ��
	//ConnectServer();
	m_SinumerikSocket.m_pSimOperdata = &m_SimData;

	InitMap();
	return TRUE;
}

int CSinumerikThread::ExitInstance()
{
	// TODO:    �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSinumerikThread, CWinThread)
END_MESSAGE_MAP()


// CSinumerikThread ��Ϣ�������
BOOL CSinumerikThread::ConnectServer()
{
	BOOL ret = FALSE;
	CString sInfor = _T("����");
	sInfor = /*m_IP +*/ _T("����");
	CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
	ret = m_SinumerikSocket.ConnectServer(m_IP, m_nPort);
	if (ret == TRUE)
	{
		sInfor.Empty();
		sInfor = /*m_IP +*/ "���ӳɹ�";
		CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
		PostThreadMessage(WM_THREADMSG1, 11, 0);
		return TRUE;
	}
	sInfor.Empty();
	sInfor = /*m_IP + */"����ʧ��";
	CDebugInfor::ShowDebugInfor(Driver, sInfor.GetBuffer(0));
	return FALSE;
}

BOOL CSinumerikThread::DisConnect()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("�ر�SOCKET"));
	m_SinumerikSocket.Disconnect();
	return FALSE;
}

void CSinumerikThread::SetStopFlag(BOOL bStopFlag)
{
	m_StopFlag = bStopFlag;
}

BOOL CSinumerikThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
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
					// �����ݴ���д����־
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
			//��ð汾��Ϣ
			PostThreadMessage(WM_THREADMSG1, 1, 0);
		}
		else if (m_bVarInfor == FALSE)
		{
			//���Ҫ�ɼ��ı�����Ϣ
			PostThreadMessage(WM_THREADMSG1, 2, 0);
		}
		else if (m_bOffSet == FALSE)
		{
			//��ñ���������Ϣ
			PostThreadMessage(WM_THREADMSG1, 3, 0);
		}
		else if (m_bVarDefine == FALSE)
		{
			//��ñ���������Ϣ
			PostThreadMessage(WM_THREADMSG1, 4, 0);
		}
		else
		{
			//�������
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
	return m_SinumerikSocket.GetServVersion();// ��÷���˰汾��
}

BOOL CSinumerikThread::GetVarInfor()
{
	return m_SinumerikSocket.GetCNCVarinfor();// ��ȡ����������Ϣ
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
	CDebugInfor::ShowDebugInfor(Driver, _T("��ʼ��Map����"));
	CString  sKey,sValue;
	sKey.Empty();
	sValue.Empty();

	sKey = _T("/Nck/Configuration/nckType");    //��������1001
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/Configuration/nckVersion");  //����汾1002
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));
	
	sKey = _T("/Nck/Configuration/maxnumGlobMachAxes"); //��Ч��1003
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numSpindles[u1]"); //ת��1004
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numGeoAxes[u1]");//������1005
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Configuration/numAuxAxes[u1]");//������1006
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/progName[u1]"); //������1007
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/workPNameLong[u1]");//�������1008
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/progStatus[u1]");  //����״̬1009
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/speedOvr[1]"); //���ᱶ��1010
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/State/numAlarms");   //������1011
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/SequencedAlarms/textIndex[1]");//��һ����1012
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/SequencedAlarms/textIndex[2]"); //�ڶ�����1013
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Nck/TopPrioAlarm/textIndex[1]");//��߱���1014
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/actToolLength1[u1]");//��ǰ���ߵĳ���1015
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Bag/State/opMode[u1]");           //�豸״̬1016
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/ProgramInfo/singleBlock[1]"); //ִ�д�����к�1017
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/actSpeed[u1,1]");      //����ת��1018
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,1]");  //�����ٶ�X1019
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,2]"); //�����ٶ�Y1020
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/actFeedRate[u1,3]"); //�����ٶ�Z1021
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[1]");//��������X1022
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[2]"); //��������Y1023
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/GeometricAxis/feedRateOvr[3]"); //��������Z1024
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/Spindle/driveLoad[u1,1]");//����1025
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

	sKey = _T("/DriveHsa/State/motorTemperature[u1]");  // ���¶�:1036
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = _T("/Channel/State/actTNumber[u1]");            //�������ߺ�1037
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));	
}

void CSinumerikThread::CopyDataToMap()
{
	CDebugInfor::ShowDebugInfor(Driver, _T("�ɼ��������ݴ洢��Map����"));
	CString  sKey, sValue;

	sKey = _T("/Nck/Configuration/nckType");    //��������
	sValue.Format(_T("%ld"),m_SimData.dwnckType);
	m_VarvalueMap[sKey] = sValue; 
	sValue.Empty();

	sKey = _T("/Nck/Configuration/nckVersion");  //����汾
	sValue.Format(_T("%.0f"), m_SimData.dbnckVersion);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	sKey = _T("/Nck/Configuration/maxnumGlobMachAxes");//���������Ч����
	sValue.Format(_T("%d"), m_SimData.nmaxnumGlobMachAxes);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//ת������
	sKey = _T("/Channel/Configuration/numSpindles[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumSpindles);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//������ͷ���������
	sKey = _T("/Channel/Configuration/numGeoAxes[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumGeoAxes);
	m_VarvalueMap[sKey] = sValue;
	sKey.Empty();

	//����������
	sKey = _T("/Channel/Configuration/numAuxAxes[u1]");
	sValue.Format(_T("%d"), m_SimData.nnumAuxAxes);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//��ǰִ�г�����
	sKey = _T("/Channel/ProgramInfo/progName[u1]");
	//sValue.Format(_T("%s"), m_SimData.cprogName);
	sValue = m_SimData.cprogName;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//�������
	sKey = _T("/Channel/ProgramInfo/workPNameLong[u1]");
	//sValue.Format(_T("%s"), m_SimData.cworkPNameLong);
	sValue = m_SimData.cworkPNameLong;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//����״̬
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

	//���ᱶ��
	sKey = _T("/Channel/Spindle/speedOvr[1]");
	sValue.Format(_T("%d"), m_SimData.nspeedOvr);
	m_VarvalueMap[sKey] = sValue;

	//��������
	sKey = _T("/Nck/State/numAlarms");
	sValue.Format(_T("%d"), m_SimData.nnumAlarms);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//��һ������
	sKey = _T("/Nck/SequencedAlarms/textIndex[1]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex1);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
	//�ڶ�������
	sKey = _T("/Nck/SequencedAlarms/textIndex[2]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex2);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//��߱�����
	sKey = _T("/Nck/TopPrioAlarm/textIndex[1]");
	sValue.Format(_T("%ld"), m_SimData.ltextIndex11);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//���߳���
	sKey = _T("/Channel/State/actToolLength1[u1]");

	//�豸״̬
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
	
	//ִ�д���
	sKey = _T("/Channel/ProgramInfo/singleBlock[1]");
	//sValue.Format(_T("%s"), m_SimData.csingleBlock);
	sValue = m_SimData.csingleBlock;
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//����ת��
	sKey = _T("/Channel/Spindle/actSpeed[u1,1]");
	sValue.Format(_T("%d"), m_SimData.dbactSpeed);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();

	//�����ٶ�
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

	//��������
	sKey = _T("/Channel/GeometricAxis/feedRateOvr[1]");
	sValue.Format(_T("%d"), m_SimData.dbfeedRateOvr1);
	m_VarvalueMap[sKey] = sValue;
	sValue.Empty();
	
/*
	sKey = "/Channel/GeometricAxis/feedRateOvr[2]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));

	sKey = "/Channel/GeometricAxis/feedRateOvr[3]";
	m_VarvalueMap.insert(std::make_pair(sKey, sValue));*/

	//���Ḻ��
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
	CString sValue(_T("��������֧�ָ���������"));
	return sValue;
}

BOOL CSinumerikThread::GetVarMapData(CVarMapData* pData)
{
	pData->m_VarvalueMap = m_VarvalueMap;
	return TRUE;
}
