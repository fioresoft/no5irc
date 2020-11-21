

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 00:14:07 2038
 */
/* Compiler settings for MyServer.idl:
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

#ifndef __irc11_h__
#define __irc11_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INo5IrcObj_FWD_DEFINED__
#define __INo5IrcObj_FWD_DEFINED__
typedef interface INo5IrcObj INo5IrcObj;

#endif 	/* __INo5IrcObj_FWD_DEFINED__ */


#ifndef __IUser_FWD_DEFINED__
#define __IUser_FWD_DEFINED__
typedef interface IUser IUser;

#endif 	/* __IUser_FWD_DEFINED__ */


#ifndef __IUsers_FWD_DEFINED__
#define __IUsers_FWD_DEFINED__
typedef interface IUsers IUsers;

#endif 	/* __IUsers_FWD_DEFINED__ */


#ifndef ___INo5IrcObjEvents_FWD_DEFINED__
#define ___INo5IrcObjEvents_FWD_DEFINED__
typedef interface _INo5IrcObjEvents _INo5IrcObjEvents;

#endif 	/* ___INo5IrcObjEvents_FWD_DEFINED__ */


#ifndef __No5IrcObj_FWD_DEFINED__
#define __No5IrcObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class No5IrcObj No5IrcObj;
#else
typedef struct No5IrcObj No5IrcObj;
#endif /* __cplusplus */

#endif 	/* __No5IrcObj_FWD_DEFINED__ */


#ifndef __User_FWD_DEFINED__
#define __User_FWD_DEFINED__

#ifdef __cplusplus
typedef class User User;
#else
typedef struct User User;
#endif /* __cplusplus */

#endif 	/* __User_FWD_DEFINED__ */


#ifndef __Users_FWD_DEFINED__
#define __Users_FWD_DEFINED__

#ifdef __cplusplus
typedef class Users Users;
#else
typedef struct Users Users;
#endif /* __cplusplus */

#endif 	/* __Users_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __INo5IrcObj_INTERFACE_DEFINED__
#define __INo5IrcObj_INTERFACE_DEFINED__

/* interface INo5IrcObj */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_INo5IrcObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9effb178-8880-434d-ac19-e74f32b375b1")
    INo5IrcObj : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendActiveChannelMsg( 
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendChannelMsg( 
            /* [in] */ BSTR channel,
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendPrivateMsg( 
            /* [in] */ BSTR to,
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendNoticeMsg( 
            /* [in] */ BSTR to,
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE JoinChannel( 
            /* [in] */ BSTR channel) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LeaveChannel( 
            /* [in] */ BSTR channel,
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Quit( 
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendPass( 
            /* [in] */ BSTR pw) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendNick( 
            /* [in] */ BSTR nick) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendUser( 
            /* [in] */ BSTR user,
            /* [in] */ BSTR name) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ListChannels( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ListChannelNames( 
            /* [in] */ BSTR channel) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTopic( 
            /* [in] */ BSTR channel) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetMode( 
            /* [in] */ BSTR NameOrChannel) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE WhoIs( 
            /* [in] */ BSTR nick) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Who( 
            /* [in] */ BSTR nick) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE WhoWas( 
            /* [in] */ BSTR nick) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Output( 
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetTimer( 
            /* [in] */ LONG id,
            /* [in] */ LONG ms,
            /* [retval][out] */ LONG *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetActiveViewName( 
            /* [retval][out] */ BSTR *res) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INo5IrcObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INo5IrcObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INo5IrcObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INo5IrcObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INo5IrcObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INo5IrcObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INo5IrcObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INo5IrcObj * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendActiveChannelMsg )( 
            INo5IrcObj * This,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendChannelMsg )( 
            INo5IrcObj * This,
            /* [in] */ BSTR channel,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendPrivateMsg )( 
            INo5IrcObj * This,
            /* [in] */ BSTR to,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendNoticeMsg )( 
            INo5IrcObj * This,
            /* [in] */ BSTR to,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *JoinChannel )( 
            INo5IrcObj * This,
            /* [in] */ BSTR channel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LeaveChannel )( 
            INo5IrcObj * This,
            /* [in] */ BSTR channel,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
            INo5IrcObj * This,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendPass )( 
            INo5IrcObj * This,
            /* [in] */ BSTR pw);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendNick )( 
            INo5IrcObj * This,
            /* [in] */ BSTR nick);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendUser )( 
            INo5IrcObj * This,
            /* [in] */ BSTR user,
            /* [in] */ BSTR name);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ListChannels )( 
            INo5IrcObj * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ListChannelNames )( 
            INo5IrcObj * This,
            /* [in] */ BSTR channel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTopic )( 
            INo5IrcObj * This,
            /* [in] */ BSTR channel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetMode )( 
            INo5IrcObj * This,
            /* [in] */ BSTR NameOrChannel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *WhoIs )( 
            INo5IrcObj * This,
            /* [in] */ BSTR nick);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Who )( 
            INo5IrcObj * This,
            /* [in] */ BSTR nick);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *WhoWas )( 
            INo5IrcObj * This,
            /* [in] */ BSTR nick);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Output )( 
            INo5IrcObj * This,
            /* [in] */ BSTR msg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetTimer )( 
            INo5IrcObj * This,
            /* [in] */ LONG id,
            /* [in] */ LONG ms,
            /* [retval][out] */ LONG *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveViewName )( 
            INo5IrcObj * This,
            /* [retval][out] */ BSTR *res);
        
        END_INTERFACE
    } INo5IrcObjVtbl;

    interface INo5IrcObj
    {
        CONST_VTBL struct INo5IrcObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INo5IrcObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INo5IrcObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INo5IrcObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INo5IrcObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define INo5IrcObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define INo5IrcObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define INo5IrcObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define INo5IrcObj_SendActiveChannelMsg(This,msg)	\
    ( (This)->lpVtbl -> SendActiveChannelMsg(This,msg) ) 

#define INo5IrcObj_SendChannelMsg(This,channel,msg)	\
    ( (This)->lpVtbl -> SendChannelMsg(This,channel,msg) ) 

#define INo5IrcObj_SendPrivateMsg(This,to,msg)	\
    ( (This)->lpVtbl -> SendPrivateMsg(This,to,msg) ) 

#define INo5IrcObj_SendNoticeMsg(This,to,msg)	\
    ( (This)->lpVtbl -> SendNoticeMsg(This,to,msg) ) 

#define INo5IrcObj_JoinChannel(This,channel)	\
    ( (This)->lpVtbl -> JoinChannel(This,channel) ) 

#define INo5IrcObj_LeaveChannel(This,channel,msg)	\
    ( (This)->lpVtbl -> LeaveChannel(This,channel,msg) ) 

#define INo5IrcObj_Quit(This,msg)	\
    ( (This)->lpVtbl -> Quit(This,msg) ) 

#define INo5IrcObj_SendPass(This,pw)	\
    ( (This)->lpVtbl -> SendPass(This,pw) ) 

#define INo5IrcObj_SendNick(This,nick)	\
    ( (This)->lpVtbl -> SendNick(This,nick) ) 

#define INo5IrcObj_SendUser(This,user,name)	\
    ( (This)->lpVtbl -> SendUser(This,user,name) ) 

#define INo5IrcObj_ListChannels(This)	\
    ( (This)->lpVtbl -> ListChannels(This) ) 

#define INo5IrcObj_ListChannelNames(This,channel)	\
    ( (This)->lpVtbl -> ListChannelNames(This,channel) ) 

#define INo5IrcObj_GetTopic(This,channel)	\
    ( (This)->lpVtbl -> GetTopic(This,channel) ) 

#define INo5IrcObj_GetMode(This,NameOrChannel)	\
    ( (This)->lpVtbl -> GetMode(This,NameOrChannel) ) 

#define INo5IrcObj_WhoIs(This,nick)	\
    ( (This)->lpVtbl -> WhoIs(This,nick) ) 

#define INo5IrcObj_Who(This,nick)	\
    ( (This)->lpVtbl -> Who(This,nick) ) 

#define INo5IrcObj_WhoWas(This,nick)	\
    ( (This)->lpVtbl -> WhoWas(This,nick) ) 

#define INo5IrcObj_Output(This,msg)	\
    ( (This)->lpVtbl -> Output(This,msg) ) 

#define INo5IrcObj_SetTimer(This,id,ms,res)	\
    ( (This)->lpVtbl -> SetTimer(This,id,ms,res) ) 

#define INo5IrcObj_GetActiveViewName(This,res)	\
    ( (This)->lpVtbl -> GetActiveViewName(This,res) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INo5IrcObj_INTERFACE_DEFINED__ */


#ifndef __IUser_INTERFACE_DEFINED__
#define __IUser_INTERFACE_DEFINED__

/* interface IUser */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IUser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("459D1B63-A9A0-44E9-9673-CF88CF8EE817")
    IUser : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pBSTR) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IUserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUser * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IUser * This,
            /* [retval][out] */ BSTR *pBSTR);
        
        END_INTERFACE
    } IUserVtbl;

    interface IUser
    {
        CONST_VTBL struct IUserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUser_get_Name(This,pBSTR)	\
    ( (This)->lpVtbl -> get_Name(This,pBSTR) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUser_INTERFACE_DEFINED__ */


#ifndef __IUsers_INTERFACE_DEFINED__
#define __IUsers_INTERFACE_DEFINED__

/* interface IUsers */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IUsers;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F7F8889B-5703-49C9-8F4D-825B6E8FC8CC")
    IUsers : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pRet) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            long n,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IUsersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUsers * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUsers * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUsers * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUsers * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUsers * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUsers * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUsers * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUsers * This,
            /* [retval][out] */ long *pRet);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IUsers * This,
            long n,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUsers * This,
            /* [retval][out] */ IUnknown **pVal);
        
        END_INTERFACE
    } IUsersVtbl;

    interface IUsers
    {
        CONST_VTBL struct IUsersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUsers_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUsers_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUsers_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUsers_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUsers_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUsers_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUsers_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUsers_get_Count(This,pRet)	\
    ( (This)->lpVtbl -> get_Count(This,pRet) ) 

#define IUsers_get_Item(This,n,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,n,pVal) ) 

#define IUsers_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUsers_INTERFACE_DEFINED__ */



#ifndef __MyServerLib_LIBRARY_DEFINED__
#define __MyServerLib_LIBRARY_DEFINED__

/* library MyServerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MyServerLib;

#ifndef ___INo5IrcObjEvents_DISPINTERFACE_DEFINED__
#define ___INo5IrcObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _INo5IrcObjEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__INo5IrcObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("e6d38d0b-0644-485a-9824-7cd1524fb6c4")
    _INo5IrcObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _INo5IrcObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _INo5IrcObjEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _INo5IrcObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _INo5IrcObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _INo5IrcObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _INo5IrcObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _INo5IrcObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _INo5IrcObjEvents * This,
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
    } _INo5IrcObjEventsVtbl;

    interface _INo5IrcObjEvents
    {
        CONST_VTBL struct _INo5IrcObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _INo5IrcObjEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _INo5IrcObjEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _INo5IrcObjEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _INo5IrcObjEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _INo5IrcObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _INo5IrcObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _INo5IrcObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___INo5IrcObjEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_No5IrcObj;

#ifdef __cplusplus

class DECLSPEC_UUID("0dba6130-ba55-4119-ad12-66f0d5392379")
No5IrcObj;
#endif

EXTERN_C const CLSID CLSID_User;

#ifdef __cplusplus

class DECLSPEC_UUID("39BDBBF4-BBBD-48F4-A6CB-B24A9288EC07")
User;
#endif

EXTERN_C const CLSID CLSID_Users;

#ifdef __cplusplus

class DECLSPEC_UUID("30AB88AE-ED7A-44C0-9215-18CFE9513D2C")
Users;
#endif
#endif /* __MyServerLib_LIBRARY_DEFINED__ */

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


