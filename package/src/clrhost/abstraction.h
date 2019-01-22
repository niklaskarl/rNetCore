#ifndef __CLRHOST_ABSTRACTION_H__
#define __CLRHOST_ABSTRACTION_H__

#include "include/hresult.h"

#include "../stringtranscoding/stringtranscoding.h"

#include <memory>

class ICLRHost;
class IAppDomain;

class ICLRHost
{
public:
    virtual HRESULT CreateAppDomain(const utf16char_t *pDllPath, std::shared_ptr<IAppDomain> *ppDomain) = 0;
};

class IAppDomain
{
public:
    virtual HRESULT CreateDelegate(const utf16char_t *pAssemblyName, const utf16char_t *pTypeName, const utf16char_t *pMethodName, void **ppDelegate) = 0;
};

#endif // __CLRHOST_ABSTRACTION_H__
