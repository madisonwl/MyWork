// SinumerikOperateData.h: interface for the SinumerikOperateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINUMERIKOPERATEDATA_H__E38DCFEF_276A_44DC_8633_B75E7545BAB0__INCLUDED_)
#define AFX_SINUMERIKOPERATEDATA_H__E38DCFEF_276A_44DC_8633_B75E7545BAB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>



class SinumerikOperateData  
{
public:
	SinumerikOperateData();
	virtual ~SinumerikOperateData();
//	BOOL GetAllVarValue(const CTypedPtrList<CPtrList,SimVarPara*> &SimVarParaList,const char* buff);
public:

	DWORD dwnckType;            //1机床类型
	double dbnckVersion;        //2软件版本
	int nmaxnumGlobMachAxes;    //3有效轴
	int nnumSpindles;           //4转轴
	int nnumGeoAxes;            //5几何轴
	int nnumAuxAxes;            //6辅助轴
	char cprogName[32];         //7程序名
	char cworkPNameLong[128];   //8主程序号 
	int nprogStatus;            //9程序状态
	int nspeedOvr;              //10主轴倍率
	int nnumAlarms;             //11报警数
	long ltextIndex1;           //12第一报警
	long ltextIndex2;           //13第二报警
	long ltextIndex11;          //14最高报警
	double dbactToolLength1;    //15当前刀具的长度
	int nopMode;                //16设备状态jog mda auto
	char csingleBlock[198];     //17执行代码和行号
	double dbactSpeed;          //18主轴转速
	double dbactFeedRate1;      //19进给速度X
	double 	dbactFeedRate2;     //20进给速度Y
	double 	dbactFeedRate3;     //21进给速度Z
	double dbfeedRateOvr1;      //22进给倍率X
	double dbfeedRateOvr2;      //23进给倍率Y
	double dbfeedRateOvr3;      //24进给倍率Z
	double dbdriveLoad;         //25负载
	double dbmaactToolBasePos1; //26machinepos1
	double	dbmaactToolBasePos2;//27
	double	dbmaactToolBasePos3;//28
	double	dbmaactToolBasePos4;//29
	double	dbmaactToolBasePos5;//30
	double	dbgeactToolBasePos1; //31/Geo-pos1
	double	dbgeactToolBasePos2;//32
	double	dbgeactToolBasePos3;//33
	double	dbgeactToolBasePos4;//34
	double	dbgeactToolBasePos5;//35
	int nmotorTemperature;        //36轴温度
	long ltoolno;                 //37工作刀号
	double dbAxisFollowErr[5];    //38-42:随动误差
	int nAxisTemperature[5];  //43-47 ：轴温度
	int nAxisCurrent[5];      //48-52 ：轴电流
	char cProductinfor[128];      //38加工信息，工单号，西飞  wl-201540608-Add
    char cWorkOrder[128];   //工单号，从加工信息中提取的工单号

	char cStopInforData[128];          //wl-201440806-Add //交互

	char cFirstAlm[256];  //第一报警
	char cSecondAlm[256]; //第二报警
	char cTopAlm[256];    //最高报警
	DWORD dWorkCount1;    //主工件加工计数
	DWORD dWorkCount2;

	char cAlarmMsg[1024];  //合成报警信息
};

typedef struct SimVarPara
{
	CString VarType;
	CString VarFlag;
	int nVarOffSet;
}SIMVARPARA, *pSIMVARPARA;

typedef struct SimVarData
{
	DWORD dwVarVal;
	int nVarVal;
	double dbVarVal;
	char* pchar;
	long lVarVal;
}SIMVARDATA,*pSimVarData;

#endif // !defined(AFX_SINUMERIKOPERATEDATA_H__E38DCFEF_276A_44DC_8633_B75E7545BAB0__INCLUDED_)
