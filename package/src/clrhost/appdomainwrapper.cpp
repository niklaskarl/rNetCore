#include "clrhosting.h"
#include "abstraction.h"

#include <utility>

clr_app_domain::clr_app_domain(const std::shared_ptr<IAppDomain> &domain) :
    m_domain(domain)
{ }

clr_app_domain::clr_app_domain::clr_app_domain(clr_app_domain &&other) noexcept :
    m_domain(std::exchange(other.m_domain ,nullptr))
{ }

clr_app_domain::~clr_app_domain()
{
    m_domain = nullptr;
}

clr_app_domain& clr_app_domain::operator=(clr_app_domain&& other) noexcept
{
    std::swap(m_domain, other.m_domain);
    return *this;
}

void *clr_app_domain::create_delegate(const utf16char_t *assembly, const utf16char_t *type, const utf16char_t *method)
{
    void *delegate = nullptr;
    HRESULT hr = m_domain->CreateDelegate(assembly, type, method, &delegate);
    if (FAILED(hr))
    {
        throw hresult_error(hr);
    }

    return delegate;
}
