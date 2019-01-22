#ifndef __COMHELPER_H__
#define __COMHELPER_H__

#define __RPCNDR_H_VERSION__

#include <stddef.h>
#include <inttypes.h>



#if _MSC_VER || __MINGW32__
#define STDAPICALLTYPE __stdcall
#define DECLSPEC_SELECTANY __declspec(selectany)
#elif __GNUC__
#define STDAPICALLTYPE
#define DECLSPEC_SELECTANY __attribute__((weak))
#endif

typedef int32_t         HRESULT;
typedef char            CHAR;
typedef CHAR *          LPSTR;
typedef const CHAR *    LPCSTR;
typedef char16_t        WCHAR;
typedef WCHAR *         LPWSTR;
typedef const WCHAR *   LPCWSTR;
typedef LPWSTR          BSTR;
typedef int             BOOL;
typedef void            VOID;
typedef VOID *          LPVOID;
typedef uint32_t        DWORD;
typedef int32_t         INT;
typedef uint32_t        UINT;
typedef int64_t         INT64;
typedef uint64_t        UINT64;
typedef int32_t         LONG;
typedef uint32_t        ULONG;
typedef uint64_t        ULONGLONG;
typedef size_t          SIZE_T;

typedef LPVOID          HMODULE;
typedef LPVOID          HINSTANCE;
typedef LPVOID          HWND;

typedef size_t UINT_PTR;
typedef ptrdiff_t INT_PTR;

#define interface struct

#define EXTERN_C extern "C"
#define MIDL_INTERFACE(uuid) struct
#define DECLSPEC_UUID(uuid)
#define STDMETHODCALLTYPE
#define STDAPI EXTERN_C HRESULT STDAPICALLTYPE

#define FAR
#define NEAR

#ifndef _In_
#define _In_
#endif // _In_

#ifndef _In_opt_
#define _In_opt_
#endif // _In_opt_

#ifndef _Out_
#define _Out_
#endif // _Out_

#ifndef _Out_opt_
#define _Out_opt_
#endif // _Out_opt_

#ifndef _Out_writes_
#define _Out_writes_(size)
#endif // _Out_writes_

#ifndef _Out_writes_z_
#define _Out_writes_z_(size)
#endif // _Out_writes_z_

#ifndef _Out_writes_to_
#define _Out_writes_to_(size,count)
#endif // _Out_writes_to_

#ifndef _Out_writes_opt_
#define _Out_writes_opt_(size)
#endif // _Out_writes_opt_

#ifndef _Out_writes_to_opt_
#define _Out_writes_to_opt_(size,count)
#endif // _Out_writes_to_opt_

struct GUID {
    ULONG Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};
typedef GUID IID;
typedef const IID &REFIID;
typedef GUID CLSID;
typedef const CLSID &REFCLSID;

#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8)  \
  EXTERN_C const IID DECLSPEC_SELECTANY itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}

typedef void *RPC_IF_HANDLE;

typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
typedef struct _PROCESS_INFORMATION PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
typedef struct _COR_GC_STATS COR_GC_STATS;

#endif //  __COMHELPER_H__
