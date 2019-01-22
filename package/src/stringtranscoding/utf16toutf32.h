#ifndef __UTF16_TO_UTF32_TRANSCODING_H__
#define __UTF16_TO_UTF32_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

template<class InputIterator>
class Utf16ToUtf32Iterator : public std::iterator<std::bidirectional_iterator_tag, utf32char_t>
{
public:
    Utf16ToUtf32Iterator(InputIterator iter) :
        m_forward(iter),
        m_backward(iter),
        m_codepoint(0)
    {
        MoveForward();
    }

    Utf16ToUtf32Iterator(const Utf16ToUtf32Iterator &iter) :
        m_forward(iter.m_forward),
        m_backward(iter.m_backward),
        m_codepoint(iter.m_codepoint)
    { }

    bool operator==(const Utf16ToUtf32Iterator &rhs) const
    {
        return m_forward == rhs.m_forward;
    }

    bool operator!=(const Utf16ToUtf32Iterator &rhs) const
    {
        return m_forward != rhs.m_forward;
    }

    Utf16ToUtf32Iterator &operator++()
    {
        m_backward = m_forward;
        MoveForward();
        return *this;
    }

    Utf16ToUtf32Iterator operator++(int)
    {
        Utf16ToUtf32Iterator tmp(*this);
        operator++();
        return tmp;
    }

    Utf16ToUtf32Iterator &operator--()
    {
        m_forward = m_backward;
        MoveBackward();
        return *this;
    }

    Utf16ToUtf32Iterator operator--(int)
    {
        Utf16ToUtf32Iterator tmp(*this);
        operator--();
        return tmp;
    }

    utf32char_t operator*() const
    {
        return m_codepoint;
    }

private:
    void MoveForward()
    {
        size_t length = 0;
        utf16char_t sequence[2];
        utf16char_t current;

        sequence[0] = *m_forward;

        do
        {
            ++length;
            ++m_forward;
            current = *m_forward;
            if (length < 2)
            {
                sequence[length] = current;
            }

        } while ((current & 0xFC00) == 0xDC00);

        if ((sequence[0] & 0xFC00) == 0xD800 && length >= 2)
        {
            if ((sequence[1] & 0xFC00) == 0xDC00)
            {
                m_codepoint = 0x10000 + (((sequence[0] & 0x03FF) << 10) | (sequence[1] & 0x03FF));
            }
            else
            {
                m_codepoint = 0;
            }
        }
        else
        {
            m_codepoint = sequence[0];
        }
    }

    void MoveBackward()
    {
        size_t length = 0;
        utf16char_t sequence[2];
        utf16char_t current;

        do
        {
            --m_backward;
            sequence[length % 2] = current = *m_backward;
            ++length;
        } while ((current & 0xFC00) == 0xDC00);

        if ((current & 0xFC00) == 0xD800)
        {
            if (length >= 2)
            {
                m_codepoint = 0x10000 + (((current & 0x03FF) << 10) | (sequence[length % 2] & 0x03FF));
            }
            else
            {
                m_codepoint = 0;
            }
        }
        else
        {
            m_codepoint = current;
        }
    }

private:
    InputIterator m_forward;
    InputIterator m_backward;
    utf32char_t m_codepoint;
};

#endif // __UTF16_TO_UTF32_TRANSCODING_H__
