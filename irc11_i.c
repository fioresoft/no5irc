

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_INo5IrcObj,0x9effb178,0x8880,0x434d,0xac,0x19,0xe7,0x4f,0x32,0xb3,0x75,0xb1);


MIDL_DEFINE_GUID(IID, LIBID_MyServerLib,0x4E40C3E9,0xCCF8,0x4451,0xBC,0x7E,0x8B,0x66,0xE7,0xD5,0x22,0xD0);


MIDL_DEFINE_GUID(IID, DIID__INo5IrcObjEvents,0xe6d38d0b,0x0644,0x485a,0x98,0x24,0x7c,0xd1,0x52,0x4f,0xb6,0xc4);


MIDL_DEFINE_GUID(CLSID, CLSID_No5IrcObj,0x0dba6130,0xba55,0x4119,0xad,0x12,0x66,0xf0,0xd5,0x39,0x23,0x79);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



