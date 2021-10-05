

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 00:14:07 2038
 */
/* Compiler settings for Marquee.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Marquee_i_h__
#define __Marquee_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMarqueeCtrl_FWD_DEFINED__
#define __IMarqueeCtrl_FWD_DEFINED__
typedef interface IMarqueeCtrl IMarqueeCtrl;

#endif 	/* __IMarqueeCtrl_FWD_DEFINED__ */


#ifndef __MarqueeCtrl_FWD_DEFINED__
#define __MarqueeCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class MarqueeCtrl MarqueeCtrl;
#else
typedef struct MarqueeCtrl MarqueeCtrl;
#endif /* __cplusplus */

#endif 	/* __MarqueeCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMarqueeCtrl_INTERFACE_DEFINED__
#define __IMarqueeCtrl_INTERFACE_DEFINED__

/* interface IMarqueeCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMarqueeCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("eb78731d-c042-4732-bbc3-ef82aa0d9ae8")
    IMarqueeCtrl : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddItem( 
            /* [in] */ BSTR bstr,
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Loop( 
            /* [in] */ VARIANT_BOOL bLoop) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Loop( 
            /* [retval][out] */ VARIANT_BOOL *bLoop) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Elapse( 
            /* [in] */ long ms) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Elapse( 
            /* [retval][out] */ long *pms) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMarqueeCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMarqueeCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMarqueeCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMarqueeCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMarqueeCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMarqueeCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMarqueeCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMarqueeCtrl * This,
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
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IMarqueeCtrl * This,
            /* [in] */ OLE_COLOR clr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IMarqueeCtrl * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddItem )( 
            IMarqueeCtrl * This,
            /* [in] */ BSTR bstr,
            /* [in] */ OLE_COLOR clr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Start )( 
            IMarqueeCtrl * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IMarqueeCtrl * This);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Loop )( 
            IMarqueeCtrl * This,
            /* [in] */ VARIANT_BOOL bLoop);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Loop )( 
            IMarqueeCtrl * This,
            /* [retval][out] */ VARIANT_BOOL *bLoop);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Elapse )( 
            IMarqueeCtrl * This,
            /* [in] */ long ms);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Elapse )( 
            IMarqueeCtrl * This,
            /* [retval][out] */ long *pms);
        
        END_INTERFACE
    } IMarqueeCtrlVtbl;

    interface IMarqueeCtrl
    {
        CONST_VTBL struct IMarqueeCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMarqueeCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMarqueeCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMarqueeCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMarqueeCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMarqueeCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMarqueeCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMarqueeCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMarqueeCtrl_put_BackColor(This,clr)	\
    ( (This)->lpVtbl -> put_BackColor(This,clr) ) 

#define IMarqueeCtrl_get_BackColor(This,pclr)	\
    ( (This)->lpVtbl -> get_BackColor(This,pclr) ) 

#define IMarqueeCtrl_AddItem(This,bstr,clr)	\
    ( (This)->lpVtbl -> AddItem(This,bstr,clr) ) 

#define IMarqueeCtrl_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IMarqueeCtrl_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IMarqueeCtrl_put_Loop(This,bLoop)	\
    ( (This)->lpVtbl -> put_Loop(This,bLoop) ) 

#define IMarqueeCtrl_get_Loop(This,bLoop)	\
    ( (This)->lpVtbl -> get_Loop(This,bLoop) ) 

#define IMarqueeCtrl_put_Elapse(This,ms)	\
    ( (This)->lpVtbl -> put_Elapse(This,ms) ) 

#define IMarqueeCtrl_get_Elapse(This,pms)	\
    ( (This)->lpVtbl -> get_Elapse(This,pms) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMarqueeCtrl_INTERFACE_DEFINED__ */



#ifndef __MarqueeLib_LIBRARY_DEFINED__
#define __MarqueeLib_LIBRARY_DEFINED__

/* library MarqueeLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MarqueeLib;

EXTERN_C const CLSID CLSID_MarqueeCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("9b827241-d8a6-4027-8a52-2ace99bd4907")
MarqueeCtrl;
#endif
#endif /* __MarqueeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


