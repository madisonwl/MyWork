// SinumerikOperateData.cpp: implementation of the SinumerikOperateData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SinumerikOperateData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SinumerikOperateData::SinumerikOperateData()
{
	memset(cprogName,0,sizeof(cprogName));  //7程序名
	memset(cworkPNameLong,0,sizeof(cworkPNameLong)); //8主程序号 
	memset(csingleBlock,0,sizeof(csingleBlock));     //17执行代码和行号
	memset(cProductinfor,0,sizeof(cProductinfor));   //38生产信息 西飞
	memset(cWorkOrder,0,sizeof(cWorkOrder));
	nnumAlarms = 0;                                  //11报警数
	ltextIndex1 = 0;                //12第一报警
	ltextIndex2 = 0;                //13第二报警
	ltextIndex11 = 0;              //14最高报警
	nmaxnumGlobMachAxes = 0;    //3有效轴
	nnumSpindles = 0;           //4转轴
	nnumGeoAxes = 0;            //5几何轴
	nnumAuxAxes = 0;            //6辅助轴
	dbnckVersion = 0;           //2软件版本
	dwnckType = 0;              //1机床类型
	nspeedOvr = 0;              //10主轴倍率
	nprogStatus = 0;            //9程序状态
	dbactToolLength1 = 0;    //15当前刀具的长度
	nopMode = 0;                //16设备状态jog mda auto
	dbactSpeed = 0;          //18主轴转速
	dbactFeedRate1 = 0;      //19进给速度X
	dbactFeedRate2 = 0;     //20进给速度Y
	dbactFeedRate3 = 0;     //21进给速度Z
	dbfeedRateOvr1 = 0;      //22进给倍率X
	dbfeedRateOvr2 = 0;      //23进给倍率Y
	dbfeedRateOvr3 = 0;      //24进给倍率Z
	dbdriveLoad = 0;         //25负载
	dbmaactToolBasePos1 = 0; //26machinepos1
	dbmaactToolBasePos2 = 0;//27
	dbmaactToolBasePos3 = 0;//28
	dbmaactToolBasePos4 = 0;//29
	dbmaactToolBasePos5 = 0;//30
	dbgeactToolBasePos1 = 0; //31/Geo-pos1
	dbgeactToolBasePos2 = 0;//32
	dbgeactToolBasePos3 = 0;//33
	dbgeactToolBasePos4 = 0;//34
	dbgeactToolBasePos5 = 0;//35
	ltoolno = 0;            //37

	memset(cStopInforData,0,sizeof(cStopInforData));     //交互  //wl-20140806-add
}

SinumerikOperateData::~SinumerikOperateData()
{

}

//BOOL SinumerikOperateData::GetAllVarValue(const CTypedPtrList<CPtrList,SimVarPara*> &SimVarParaList, const char* buff)
//{
/*	int nVarNum = 0;
//	nVarNum = *(UINT*)(buff+sizeof(double)+sizeof(int));

	POSITION pos = NULL;
	pos = 	SimVarParaList.GetHeadPosition();

	int nAllOffSet = 0;   //总的偏移量
	int nOffSet = 0;
	CString strVarType;   //类型
	CString strVarFlag;

// 	DWORD dwVarVal;
// 	int nVarVal;
// 	double dbVarVal;
// 	char* pchar = NULL;
// 	long lVarVal;

	int nFlag;

	SimVarPara *pvarpara1 = NULL;
	SimVarData *psimvardata = new SimVarData;
	while(pos)
	{
		pvarpara1 = SimVarParaList.GetAt(pos);
		nOffSet =  pvarpara1->nVarOffSet; //偏移量
		strVarType = pvarpara1->VarType;  //1:DWORD;2:int;3:double 4:char; 5:long
		
		if (strVarType=="1")
		{
			psimvardata->dwVarVal = 	*(DWORD*)(buff+sizeof(double)+sizeof(int)*2 + nAllOffSet);
			nAllOffSet+=nOffSet;	
		}
		else if (strVarType=="2")
		{
			psimvardata->nVarVal = 	*(UINT*)(buff+sizeof(double)+sizeof(int)*2 + nAllOffSet);
			nAllOffSet+=nOffSet;	
		}
		else if (strVarType=="3")
		{
			psimvardata->dbVarVal = 	*(double*)(buff+sizeof(double)+sizeof(int)*2 + nAllOffSet);
			nAllOffSet+=nOffSet;	
		}
		else if (strVarType=="4")
		{
			psimvardata->pchar = new char[nOffSet];
			memcpy(psimvardata->pchar,buff+sizeof(double)+sizeof(int)*2 + nAllOffSet,nOffSet);
			nAllOffSet+=nOffSet;
		}
		else if (strVarType=="5")
		{
			psimvardata->lVarVal = 	*(long*)(buff+sizeof(double)+sizeof(int)*2 + nAllOffSet);
			nAllOffSet+=nOffSet;	
		}

		strVarFlag = pvarpara1->VarFlag;
		nFlag = atoi(strVarFlag);
		switch(nFlag)
		{
		case 1001:
			dwnckType = psimvardata->dwVarVal;
			break;
		case 1002:
			dbnckVersion = psimvardata->dbVarVal;
			break;
		case 1003:
			nmaxnumGlobMachAxes = psimvardata->nVarVal;
			break;
		case 1004:
			nnumSpindles = psimvardata->nVarVal;
			break;
		case 1005:
			nnumGeoAxes = psimvardata->nVarVal;
			break;
		case 1006:
			nnumAuxAxes = psimvardata->nVarVal;
			break;
		case 1007:
			memcpy(cprogName,psimvardata->pchar,nOffSet);
			delete [] psimvardata->pchar;
		    psimvardata->pchar = NULL;
			break;
		case 1008:
			memcpy(cworkPNameLong,psimvardata->pchar,nOffSet);
			delete [] psimvardata->pchar;
		    psimvardata->pchar = NULL;
			break;
		case 1009:
			nprogStatus = psimvardata->nVarVal;
			break;
		case 1010:
			nspeedOvr = psimvardata->nVarVal;
			break;
		case 1011:
			nnumAlarms = psimvardata->nVarVal;
			break;
		case 1012:
			ltextIndex1 = psimvardata->lVarVal;
			break;
		case 1013:
			ltextIndex2 = psimvardata->lVarVal;
			break;
		case 1014:
			ltextIndex11 = psimvardata->lVarVal;
			break;
		case 1015:
			dbactToolLength1 = psimvardata->dbVarVal;
			break;
		case 1016:
			nopMode = psimvardata->nVarVal;
			break;
		case 1017:
			memcpy(cworkPNameLong,psimvardata->pchar,nOffSet);
			delete [] psimvardata->pchar;
		    psimvardata->pchar = NULL;
			break;
		case 1018:
			dbactSpeed = psimvardata->dbVarVal;
			break;
		case 1019:
			dbactFeedRate1 = psimvardata->dbVarVal;
			break;
		case 1020:
			dbactFeedRate2 = psimvardata->dbVarVal;
			break;
		case 1021:
			dbactFeedRate3 = psimvardata->dbVarVal;
			break;
		case 1022:
			dbfeedRateOvr1 = psimvardata->dbVarVal;
			break;
		case 1023:
			dbfeedRateOvr2 = psimvardata->dbVarVal;
			break;
		case 1024:
			dbfeedRateOvr3 = psimvardata->dbVarVal;
			break;
		case 1025:
			dbdriveLoad = psimvardata->dbVarVal;
			break;
		case 1026:
			dbmaactToolBasePos1 = psimvardata->dbVarVal;
			break;
		case 1027:
			dbmaactToolBasePos2 = psimvardata->dbVarVal;
			break;
		case 1028:
			dbmaactToolBasePos3 = psimvardata->dbVarVal;
			break;
		case 1029:
			dbmaactToolBasePos4 = psimvardata->dbVarVal;
			break;
		case 1030:
			dbmaactToolBasePos5 = psimvardata->dbVarVal;
			break;
		case 1031:
			dbgeactToolBasePos1 = psimvardata->dbVarVal;
			break;
		case 1032:
			dbgeactToolBasePos2 = psimvardata->dbVarVal;
			break;
		case 1033:
			dbgeactToolBasePos3 = psimvardata->dbVarVal;
			break;
		case 1034:
			dbgeactToolBasePos4 = psimvardata->dbVarVal;
			break;
		case 1035:
			dbgeactToolBasePos5 = psimvardata->dbVarVal;
			break;
		case 1036:
			nmotorTemperature = psimvardata->nVarVal;
			break;
		default:
			break;
		}
		SimVarParaList.GetNext(pos);
	} */

//	return FALSE;
//}
