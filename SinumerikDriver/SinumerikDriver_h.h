

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Oct 19 11:22:00 2016
 */
/* Compiler settings for SinumerikDriver.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SinumerikDriver_h_h__
#define __SinumerikDriver_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISinumerikDriverCom_FWD_DEFINED__
#define __ISinumerikDriverCom_FWD_DEFINED__
typedef interface ISinumerikDriverCom ISinumerikDriverCom;

#endif 	/* __ISinumerikDriverCom_FWD_DEFINED__ */


#ifndef __SinumerikDriverCom_FWD_DEFINED__
#define __SinumerikDriverCom_FWD_DEFINED__

#ifdef __cplusplus
typedef class SinumerikDriverCom SinumerikDriverCom;
#else
typedef struct SinumerikDriverCom SinumerikDriverCom;
#endif /* __cplusplus */

#endif 	/* __SinumerikDriverCom_FWD_DEFINED__ */


#ifndef __ISinumerikDriverSer_FWD_DEFINED__
#define __ISinumerikDriverSer_FWD_DEFINED__
typedef interface ISinumerikDriverSer ISinumerikDriverSer;

#endif 	/* __ISinumerikDriverSer_FWD_DEFINED__ */


#ifndef __SinumerikDriverSer_FWD_DEFINED__
#define __SinumerikDriverSer_FWD_DEFINED__

#ifdef __cplusplus
typedef class SinumerikDriverSer SinumerikDriverSer;
#else
typedef struct SinumerikDriverSer SinumerikDriverSer;
#endif /* __cplusplus */

#endif 	/* __SinumerikDriverSer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SinumerikDriver_LIBRARY_DEFINED__
#define __SinumerikDriver_LIBRARY_DEFINED__

/* library SinumerikDriver */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SinumerikDriver;

#ifndef __ISinumerikDriverCom_DISPINTERFACE_DEFINED__
#define __ISinumerikDriverCom_DISPINTERFACE_DEFINED__

/* dispinterface ISinumerikDriverCom */
/* [uuid] */ 


EXTERN_C const IID DIID_ISinumerikDriverCom;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A5DF808F-92DA-405B-8728-FE2B45F39FD4")
    ISinumerikDriverCom : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISinumerikDriverComVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISinumerikDriverCom * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISinumerikDriverCom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISinumerikDriverCom * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISinumerikDriverCom * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISinumerikDriverCom * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISinumerikDriverCom * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISinumerikDriverCom * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ISinumerikDriverComVtbl;

    interface ISinumerikDriverCom
    {
        CONST_VTBL struct ISinumerikDriverComVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISinumerikDriverCom_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISinumerikDriverCom_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISinumerikDriverCom_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISinumerikDriverCom_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISinumerikDriverCom_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISinumerikDriverCom_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISinumerikDriverCom_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISinumerikDriverCom_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SinumerikDriverCom;

#ifdef __cplusplus

class DECLSPEC_UUID("6DCF3119-9C41-4B14-A6BF-DC3CEDB3C847")
SinumerikDriverCom;
#endif

#ifndef __ISinumerikDriverSer_DISPINTERFACE_DEFINED__
#define __ISinumerikDriverSer_DISPINTERFACE_DEFINED__

/* dispinterface ISinumerikDriverSer */
/* [uuid] */ 


EXTERN_C const IID DIID_ISinumerikDriverSer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8E729AA6-9E85-45E2-BC41-76FAE56EC135")
    ISinumerikDriverSer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISinumerikDriverSerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISinumerikDriverSer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISinumerikDriverSer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISinumerikDriverSer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISinumerikDriverSer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISinumerikDriverSer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISinumerikDriverSer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISinumerikDriverSer * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ISinumerikDriverSerVtbl;

    interface ISinumerikDriverSer
    {
        CONST_VTBL struct ISinumerikDriverSerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISinumerikDriverSer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISinumerikDriverSer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISinumerikDriverSer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISinumerikDriverSer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISinumerikDriverSer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISinumerikDriverSer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISinumerikDriverSer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISinumerikDriverSer_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SinumerikDriverSer;

#ifdef __cplusplus

class DECLSPEC_UUID("4CB51EDE-4B17-4ED2-9CD7-D62F02FB9E32")
SinumerikDriverSer;
#endif
#endif /* __SinumerikDriver_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


