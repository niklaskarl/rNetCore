#ifndef __UTF32_TO_UTF16_TRANSCODING_H__
#define __UTF32_TO_UTF16_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

template<class InputIterator>
class Utf32ToUtf16Iterator : public std::iterator<std::bidirectional_iterator_tag, utf16char_t>
{
public:
    Utf32ToUtf16Iterator(InputIterator iter) :
        m_iter(iter),
        m_sequence(),
        m_sequenceLength(0),
        m_unit(0)
    {
        UpdateSequence();
    }

    Utf32ToUtf16Iterator(const Utf32ToUtf16Iterator &iter) :
        m_iter(iter.m_iter),
        m_sequence(iter.m_sequence),
        m_sequenceLength(iter.m_sequenceLength),
        m_unit(iter.m_unit)
    { }

    bool operator==(const Utf32ToUtf16Iterator &rhs) const
    {
        return m_iter == rhs.m_iter && m_unit == rhs.m_unit;
    }

    bool operator!=(const Utf32ToUtf16Iterator &rhs) const
    {
        return m_iter != rhs.m_iter || m_unit != rhs.m_unit;
    }

    Utf32ToUtf16Iterator &operator++()
    {
        if (m_unit < m_sequenceLength - 1)
        {
            ++m_unit;
        }
        else
        {
            ++m_iter;
            UpdateSequence();
            m_unit = 0;
        }

        return *this;
    }

    Utf32ToUtf16Iterator operator++(int)
    {
        Utf32ToUtf16Iterator tmp(*this);
        operator++();
        return tmp;
    }

    Utf32ToUtf16Iterator &operator--()
    {
        if (m_unit > 0)
        {
            --m_unit;
        }
        else
        {
            --m_iter;
            UpdateSequence();
            m_unit = m_sequenceLength - 1;
        }

        return *this;
    }

    Utf32ToUtf16Iterator operator--(int)
    {
        Utf32ToUtf16Iterator tmp(*this);
        operator--();
        return tmp;
    }

    utf16char_t operator*() const
    {
        return m_sequence.units[m_unit];
    }

private:
    void UpdateSequence()
    {
        utf32char_t codepoint = *m_iter;
        if (codepoint < 0x10000)
        {
            // Only one unit required.
            m_sequence.units[0] = (utf16char_t)codepoint;
            m_sequenceLength = 1;
        }
        else if (codepoint < 0x110000)
        {
            // Too small to fit in one unit. Split into low- and high-surrogate.
            m_sequence.units[0] = (utf16char_t)((codepoint - 0x10000) / 0x0400) + 0xD800;
            m_sequence.units[1] = (utf16char_t)(codepoint % 0x0400) + 0xDC00;
            m_sequenceLength = 2;
        }
        else
        {
            m_sequence.units[0] = 0;
            m_sequenceLength = 1;
        }
    }

private:
    InputIterator m_iter;
    struct
    {
        utf16char_t units[2];
    } m_sequence;
    uint8_t m_sequenceLength;
    uint8_t m_unit;

};

#endif // __UTF32_TO_UTF16_TRANSCODING_H__
