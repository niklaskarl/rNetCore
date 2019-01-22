#ifndef __UTF8_TO_UTF32_TRANSCODING_H__
#define __UTF8_TO_UTF32_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

template<class InputIterator>
class Utf8ToUtf32Iterator : public std::iterator<std::bidirectional_iterator_tag, utf32char_t>
{
public:
    Utf8ToUtf32Iterator(InputIterator iter) :
        m_forward(iter),
        m_backward(iter),
        m_codepoint(0)
    {
        MoveForward();
    }

    Utf8ToUtf32Iterator(const Utf8ToUtf32Iterator &iter) :
        m_forward(iter.m_forward),
        m_backward(iter.m_backward),
        m_codepoint(iter.m_codepoint)
    { }

    bool operator==(const Utf8ToUtf32Iterator &rhs) const
    {
        return m_forward == rhs.m_forward;
    }

    bool operator!=(const Utf8ToUtf32Iterator &rhs) const
    {
        return m_forward != rhs.m_forward;
    }

    Utf8ToUtf32Iterator &operator++()
    {
        m_backward = m_forward;
        MoveForward();
        return *this;
    }

    Utf8ToUtf32Iterator operator++(int)
    {
        Utf8ToUtf32Iterator tmp(*this);
        operator++();
        return tmp;
    }

    Utf8ToUtf32Iterator &operator--()
    {
        m_forward = m_backward;
        MoveBackward();
        return *this;
    }

    Utf8ToUtf32Iterator operator--(int)
    {
        Utf8ToUtf32Iterator tmp(*this);
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
        utf8char_t sequence[4];
        utf8char_t current;

        sequence[0] = *m_forward;

        do
        {
            ++length;
            ++m_forward;
            current = *m_forward;
            if (length < 4)
            {
                sequence[length] = current;
            }

        } while ((current & 0b11000000) == 0b10000000);

        if ((sequence[0] & 0b10000000) == 0)
        {
            m_codepoint = sequence[0];
        }
        else if ((sequence[0] & 0b11100000) == 0b11000000 && length >= 2)
        {
            m_codepoint = ((sequence[0] & 0b00011111) << 6) |
                (sequence[1] & 0b00111111);
        }
        else if ((sequence[0] & 0b11110000) == 0b11100000 && length >= 3)
        {
            m_codepoint = ((sequence[0] & 0b00001111) << 12) |
                ((sequence[1] & 0b00111111) << 6) |
                (sequence[2] & 0b00111111);
        }
        else if ((sequence[0] & 0b11111000) == 0b11110000 && length >= 4)
        {
            m_codepoint = ((sequence[0] & 0b00000111) << 18) |
                ((sequence[1] & 0b00111111) << 12) |
                ((sequence[2] & 0b00111111) << 6) |
                (sequence[3] & 0b00111111);
        }
        else
        {
            // not a valid utf-8 sequence
            m_codepoint = 0;
        }
    }

    void MoveBackward()
    {
        size_t length = 0;
        utf8char_t sequence[4];
        utf8char_t current;

        do
        {
            --m_backward;
            sequence[length % 4] = current = *m_backward;
            ++length;
        } while ((current & 0b11000000) == 0b10000000);

        if ((current & 0b10000000) == 0)
        {
            m_codepoint = current;
        }
        else if ((current & 0b11100000) == 0b11000000)
        {
            if (length >= 2)
            {
                m_codepoint = ((current & 0b00011111) << 6) |
                    (sequence[(length - 2) % 4] & 0b00111111);
            }
            else
            {
                m_codepoint = 0;
            }
        }
        else if ((current & 0b11110000) == 0b11100000)
        {
            if (length >= 3)
            {
                m_codepoint = ((current & 0b00001111) << 12) |
                    ((sequence[(length - 2) % 4] & 0b00111111) << 6) |
                    (sequence[(length - 3) % 4] & 0b00111111);
            }
            else
            {
                m_codepoint = 0;
            }
        }
        else if ((current & 0b11111000) == 0b11110000)
        {
            if (length >= 4)
            {
                m_codepoint = ((current & 0b00000111) << 18) |
                    ((sequence[(length - 2) % 4] & 0b00111111) << 12) |
                    ((sequence[(length - 3) % 4] & 0b00111111) << 6) |
                    (sequence[length % 4] & 0b00111111);
            }
            else
            {
                m_codepoint = 0;
            }
        }
    }

private:
    InputIterator m_forward;
    InputIterator m_backward;
    utf32char_t m_codepoint;
};

#endif // __UTF8_TO_UTF32_TRANSCODING_H__
