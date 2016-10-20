#pragma once
#include <map>
#include <afxstr.h>
class CVarMapData
{
public:
	CVarMapData();
	~CVarMapData();
	std::map<CString, CString> m_VarvalueMap;  //存储变量名和值
};
