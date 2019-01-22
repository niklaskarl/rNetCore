#ifndef __COMPTR_H__
#define __COMPTR_H__

#include <utility>

template<typename T>
class ComPtr {
public:
    ComPtr() :
        m_ptr(NULL)
    { }

    ComPtr(T *ptr) :
        m_ptr(ptr)
    {
        if (m_ptr) m_ptr->AddRef();
    }

    ComPtr<T>(const ComPtr<T> &other) :
        m_ptr(other.m_ptr)
    {
        if (m_ptr) m_ptr->AddRef();
    }

    ComPtr<T>(ComPtr<T> &&other) :
        m_ptr(std::exchange(other.m_ptr, NULL))
    { }

    ~ComPtr<T>()
    {
        if (m_ptr) m_ptr->Release();
    }

    inline const ComPtr<T> &operator=(T *ptr)
    {
        if (m_ptr) m_ptr->Release();
        m_ptr = ptr;
        if (m_ptr) m_ptr->AddRef();

        return *this;
    }

    inline const ComPtr<T> &operator=(const ComPtr<T> &other)
    {
        return operator=(other.m_ptr);
    }

    inline const ComPtr<T> &operator=(ComPtr<T> &&other)
    {
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }

    inline T **operator&()
    {
        return &m_ptr;
    }

    inline T *operator->() const
    {
        return m_ptr;
    }

    inline operator T*() const
    {
        return m_ptr;
    }

    inline operator bool() const
    {
        return (bool)m_ptr;
    }

    inline bool operator!() const
    {
        return !m_ptr;
    }

    inline friend bool operator==(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr == rhs;
    }

    inline friend bool operator==(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs == rhs.m_ptr;
    }

    inline friend bool operator==(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr == rhs.m_ptr;
    }

    inline friend bool operator!=(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr != rhs;
    }

    inline friend bool operator!=(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs != rhs.m_ptr;
    }

    inline friend bool operator!=(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr != rhs.m_ptr;
    }

    inline friend bool operator>(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr > rhs;
    }

    inline friend bool operator>(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs > rhs.m_ptr;
    }

    inline friend bool operator>(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr > rhs.m_ptr;
    }

    inline friend bool operator>=(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr >= rhs;
    }

    inline friend bool operator>=(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs >= rhs.m_ptr;
    }

    inline friend bool operator>=(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr >= rhs.m_ptr;
    }

    inline friend bool operator<(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr < rhs;
    }

    inline friend bool operator<(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs < rhs.m_ptr;
    }

    inline friend bool operator<(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr < rhs.m_ptr;
    }

    inline friend bool operator<=(const ComPtr<T> &lhs, T *rhs)
    {
        return lhs.m_ptr <= rhs;
    }

    inline friend bool operator<=(T *lhs, const ComPtr<T> &rhs)
    {
        return lhs <= rhs.m_ptr;
    }

    inline friend bool operator<=(const ComPtr<T> &lhs, const ComPtr<T> &rhs)
    {
        return lhs.m_ptr <= rhs.m_ptr;
    }

private:
    T *m_ptr;
};

#endif // __COMPTR_H__
