#pragma once
#include "Unknwn.h"
#include "NetMachDriverData.h"
//#include "DataCallBack.h"
#include "SinumerikDriver_h.h"

static const IID IID_NetMach1 = { 0x2525ab33, 0xdaf2, 0x450f, { 0xa3, 0x85, 0xfd, 0xaf, 0xf7, 0xb6, 0x66, 0x49 } };

class INetMach : public IUnknown
{
public:
	//NetMach Interface
	virtual BOOL STDMETHODCALLTYPE Start(const CHAR* szMachIP, int nPort) = 0;
	virtual BOOL STDMETHODCALLTYPE Stop() = 0;
	virtual CString STDMETHODCALLTYPE GetSingleVarValue(const CHAR* sVarFlag) = 0;
	virtual CString STDMETHODCALLTYPE GetSingleVarValue(const int nIndex)=0;
	virtual BOOL STDMETHODCALLTYPE GetVarMapData(CVarMapData* pData) = 0;

	//callback
	virtual HRESULT STDMETHODCALLTYPE Advise (ISinuDataCallBack * pCallBack, long * pdwCookie) = 0;						
	virtual HRESULT STDMETHODCALLTYPE Unadvise(long dwCookie) = 0;
	virtual HRESULT STDMETHODCALLTYPE Add(long n1, long n2) = 0;

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObj) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
};

