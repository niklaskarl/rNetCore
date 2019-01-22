#include "appdomain_dotnet.h"

AppDomainDotnet::AppDomainDotnet(const std::shared_ptr<ICLRHost> &pHost, ICLRRuntimeHost2 *pRuntimeHost, uint32_t domainId) :
    m_pHost(pHost),
    m_pRuntimeHost(pRuntimeHost),
    m_domainId(domainId)
{ }

AppDomainDotnet::~AppDomainDotnet()
{ }

HRESULT AppDomainDotnet::CreateDelegate(const utf16char_t *pAssemblyName, const utf16char_t *pTypeName, const utf16char_t *pMethodName, void **ppDelegate)
{
    HRESULT hr = ppDelegate ? S_OK : E_POINTER;
    if (SUCCEEDED(hr))
    {
        ICLRRuntimeHost2 *rt = m_pRuntimeHost;

        // std::string assemblyName = utf16_to_utf8(pAssemblyName);
        // std::string typeName = utf16_to_utf8(pTypeName);
        // std::string methodName = utf16_to_utf8(pMethodName);
        // hr = rt->CreateDelegate(m_domainId, (LPCWSTR)assemblyName.c_str(), (LPCWSTR)typeName.c_str(), (LPCWSTR)methodName.c_str(), (INT_PTR *)ppDelegate);

        hr = rt->CreateDelegate(m_domainId, pAssemblyName, pTypeName, pMethodName, (INT_PTR *)ppDelegate);
    }

    return hr;
}
