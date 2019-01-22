#ifndef __UTF32_TO_UTF8_TRANSCODING_H__
#define __UTF32_TO_UTF8_TRANSCODING_H__

#include <inttypes.h>

#include "stringtypes.h"
#include "stringtranscoding.h"

template<class InputIterator>
class Utf32ToUtf8Iterator : public std::iterator<std::bidirectional_iterator_tag, utf8char_t>
{
public:
    Utf32ToUtf8Iterator(InputIterator iter) :
        m_iter(iter),
        m_sequence(),
        m_sequenceLength(0),
        m_unit(0)
    {
        UpdateSequence();
    }

    Utf32ToUtf8Iterator(const Utf32ToUtf8Iterator &iter) :
        m_iter(iter.m_iter),
        m_sequence(iter.m_sequence),
        m_sequenceLength(iter.m_sequenceLength),
        m_unit(iter.m_unit)
    { }

    bool operator==(const Utf32ToUtf8Iterator &rhs) const
    {
        return m_iter == rhs.m_iter && m_unit == rhs.m_unit;
    }

    bool operator!=(const Utf32ToUtf8Iterator &rhs) const
    {
        return m_iter != rhs.m_iter || m_unit != rhs.m_unit;
    }

    Utf32ToUtf8Iterator &operator++()
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

    Utf32ToUtf8Iterator operator++(int)
    {
        Utf32ToUtf8Iterator tmp(*this);
        operator++();
        return tmp;
    }

    Utf32ToUtf8Iterator &operator--()
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

    Utf32ToUtf8Iterator operator--(int)
    {
        Utf32ToUtf8Iterator tmp(*this);
        operator--();
        return tmp;
    }

    utf8char_t operator*() const
    {
        return m_sequence.units[m_unit];
    }

private:
    void UpdateSequence()
    {
        utf32char_t codepoint = *m_iter;
        if (codepoint < 0x0080)
        {
            m_sequence.units[0] = (uint8_t)codepoint;
            m_sequenceLength = 1;
        }
        else if (codepoint < 0x0800)
        {
            m_sequence.units[0] = 0b11000000 | ((codepoint >> 6) & 0b00011111);
            m_sequence.units[1] = 0b10000000 | ((codepoint >> 0) & 0b00111111);
            m_sequenceLength = 2;
        }
        else if (codepoint < 0x10000)
        {
            m_sequence.units[0] = 0b11100000 | ((codepoint >> 12) & 0b00001111);
            m_sequence.units[1] = 0b10000000 | ((codepoint >>  6) & 0b00111111);
            m_sequence.units[2] = 0b10000000 | ((codepoint >>  0) & 0b00111111);
            m_sequenceLength = 3;
        }
        else if (codepoint < 0x200000)
        {
            m_sequence.units[0] = 0b11110000 | ((codepoint >> 18) & 0b00000111);
            m_sequence.units[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
            m_sequence.units[2] = 0b10000000 | ((codepoint >>  6) & 0b00111111);
            m_sequence.units[3] = 0b10000000 | ((codepoint >>  0) & 0b00111111);
            m_sequenceLength = 4;
        }
    }

private:
    InputIterator m_iter;
    struct
    {
        utf8char_t units[4];
    } m_sequence;
    uint8_t m_sequenceLength;
    uint8_t m_unit;

};

#endif // __UTF32_TO_UTF8_TRANSCODING_H__
