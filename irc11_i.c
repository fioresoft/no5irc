

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


MIDL_DEFINE_GUID(IID, IID_IUser,0x459D1B63,0xA9A0,0x44E9,0x96,0x73,0xCF,0x88,0xCF,0x8E,0xE8,0x17);


MIDL_DEFINE_GUID(IID, IID_IUsers,0xF7F8889B,0x5703,0x49C9,0x8F,0x4D,0x82,0x5B,0x6E,0x8F,0xC8,0xCC);


MIDL_DEFINE_GUID(IID, LIBID_MyServerLib,0x4E40C3E9,0xCCF8,0x4451,0xBC,0x7E,0x8B,0x66,0xE7,0xD5,0x22,0xD0);


MIDL_DEFINE_GUID(IID, DIID__INo5IrcObjEvents,0xe6d38d0b,0x0644,0x485a,0x98,0x24,0x7c,0xd1,0x52,0x4f,0xb6,0xc4);


MIDL_DEFINE_GUID(CLSID, CLSID_No5IrcObj,0x0dba6130,0xba55,0x4119,0xad,0x12,0x66,0xf0,0xd5,0x39,0x23,0x79);


MIDL_DEFINE_GUID(CLSID, CLSID_User,0x39BDBBF4,0xBBBD,0x48F4,0xA6,0xCB,0xB2,0x4A,0x92,0x88,0xEC,0x07);


MIDL_DEFINE_GUID(CLSID, CLSID_Users,0x30AB88AE,0xED7A,0x44C0,0x92,0x15,0x18,0xCF,0xE9,0x51,0x3D,0x2C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



