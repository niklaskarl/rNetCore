#ifndef __HRESULT_H__
#define __HRESULT_H__

#include <exception>
#include <inttypes.h>
#include <iomanip>
#include <string>
#include <sstream>

typedef int32_t HRESULT;

class hresult_error : public std::exception
{
public:
    hresult_error(HRESULT hr) noexcept :
        m_hr(hr),
        m_what()
    {
        std::ostringstream stream;
        stream << "Failed with HRESULT 0x";
        stream << std::setw(8) << std::setfill('0') << std::hex << m_hr;
        m_what = stream.str();
    }

    hresult_error(const hresult_error &other) noexcept :
        m_hr(other.m_hr),
        m_what(other.m_what)
    { }

    ~hresult_error() noexcept
    { }

    hresult_error &operator=(const hresult_error &rhs) noexcept
    {
        m_hr = rhs.m_hr;
        return *this;
    }

    const char *what() const noexcept
    {
        return m_what.c_str();
    }

private:
    HRESULT m_hr;
    std::string m_what;
};

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define S_OK ((HRESULT)0)

#define E_FAIL ((HRESULT)0x80004005)
#define E_POINTER ((HRESULT)0x80004003)

#endif // __HRESULT_H__
