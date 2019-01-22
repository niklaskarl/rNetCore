#ifndef __UNKNWN_H__
#define __UNKNWN_H__

#include "wintypes.h"

MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject) = 0;

    virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

    virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

    /*template<class Q>
    HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp)
    {
        return QueryInterface(__uuidof(Q), (void **)pp);
    }*/
};

MIDL_INTERFACE("0000000c-0000-0000-C000-000000000046")
IStream;

#endif // __UNKNWN_H__
