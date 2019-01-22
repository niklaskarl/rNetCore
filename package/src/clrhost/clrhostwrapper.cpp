#include "clrhosting.h"
#include "abstraction.h"

#include <utility>

clr_host::clr_host(const std::shared_ptr<ICLRHost> host) :
    m_host(host)
{ }

clr_host::clr_host(clr_host &&other) noexcept :
    m_host(std::exchange(other.m_host, nullptr))
{ }

clr_host::~clr_host()
{
    m_host = nullptr;
}

clr_host& clr_host::operator=(clr_host&& other) noexcept
{
    std::swap(m_host, other.m_host);
    return *this;
}

clr_app_domain clr_host::create_app_domain(const utf16char_t *dll_path)
{
    std::shared_ptr<IAppDomain> domain;
    HRESULT hr = m_host->CreateAppDomain(dll_path, &domain);
    if (FAILED(hr))
    {
        throw hresult_error(hr);
    }

    return clr_app_domain(domain);
}
