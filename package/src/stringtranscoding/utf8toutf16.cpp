#include "utf8toutf16.h"

uint32_t Utf8SequenceToUnicodeCodepoint(const utf8char_t *sequence, const size_t &maxLength)
{
    uint32_t codepoint = 0;
    if (maxLength >= 1 && (sequence[0] & 0b10000000) == 0)
    {
        codepoint = *sequence;
    }
    else if (maxLength >= 2 &&
        (sequence[0] & 0b11100000) == 0b11000000 &&
        (sequence[1] & 0b11000000) == 0b10000000)
    {
        codepoint = ((sequence[0] & 0b00011111) << 6) |
            (sequence[1] & 0b00111111);
    }
    else if (maxLength >= 3 &&
        (sequence[0] & 0b11110000) == 0b11100000 &&
        (sequence[1] & 0b11000000) == 0b10000000 &&
        (sequence[2] & 0b11000000) == 0b10000000)
    {
        codepoint = ((sequence[0] & 0b00001111) << 12) |
            ((sequence[1] & 0b00111111) << 6) |
            (sequence[2] & 0b00111111);
    }
    else if (maxLength >= 4 &&
        (sequence[0] & 0b11111000) == 0b11110000 &&
        (sequence[1] & 0b11000000) == 0b10000000 &&
        (sequence[2] & 0b11000000) == 0b10000000 &&
        (sequence[3] & 0b11000000) == 0b10000000)
    {
        codepoint = ((sequence[0] & 0b00000111) << 18) |
            ((sequence[1] & 0b00111111) << 12) |
            ((sequence[2] & 0b00111111) << 6) |
            (sequence[3] & 0b00111111);
    }

    return codepoint;
}

uint8_t GetUtf8SequenceLength(const utf8char_t *sequence, const size_t &maxLength)
{
    uint8_t length = 1;
    if (maxLength >= 1 && (sequence[0] & 0b10000000) == 0)
    {
        length = 1;
    }
    else if (maxLength >= 2 &&
        (sequence[0] & 0b11100000) == 0b11000000 &&
        (sequence[1] & 0b11000000) == 0b10000000)
    {
        length = 2;
    }
    else if (maxLength >= 3 &&
        (sequence[0] & 0b11110000) == 0b11100000 &&
        (sequence[1] & 0b11000000) == 0b10000000 &&
        (sequence[2] & 0b11000000) == 0b10000000)
    {
        length = 3;
    }
    else if (maxLength >= 4 &&
        (sequence[0] & 0b11111000) == 0b11110000 &&
        (sequence[1] & 0b11000000) == 0b10000000 &&
        (sequence[2] & 0b11000000) == 0b10000000 &&
        (sequence[3] & 0b11000000) == 0b10000000)
    {
        length = 4;
    }

    return length;
}

uint8_t GetUtf16SequenceLength(uint32_t codepoint)
{
    uint8_t length = 0;
    if (codepoint < 0x10000)
    {
        length = 1;
    }
    else if (codepoint < 0x110000)
    {
        length = 2;
    }

    return length;
}

uint32_t UnicodeCodepointToUtf16Sequence(const uint32_t &codepoint)
{
    volatile uint32_t sequence = 0;
    uint16_t *units = (uint16_t *)&sequence;
    if (codepoint < 0x10000)
    {
        // Only one unit required.
        units[0] = (uint16_t)codepoint;
    }
    else if (codepoint < 0x110000)
    {
        // Too small to fit in one unit. Split into low- and high-surrogate.
        units[0] = ((codepoint - 0x10000) / 0x0400) + 0xD800;
        units[1] = (codepoint % 0x0400) + 0xDC00;
    }

    return sequence;
}

Utf8ToUtf16Iterator::Utf8ToUtf16Iterator(const utf8char_t *str, ptrdiff_t len, ptrdiff_t pos) :
    m_str(str),
    m_len(len),
    m_pos(pos),
    m_subPos(0)
{ }

Utf8ToUtf16Iterator::Utf8ToUtf16Iterator(const Utf8ToUtf16Iterator &iter) :
    m_str(iter.m_str),
    m_len(iter.m_len),
    m_pos(iter.m_pos),
    m_subPos(iter.m_subPos)
{ }

Utf8ToUtf16Iterator::operator bool() const
{
    return m_str && (m_len >= 0 ? m_pos < m_len : operator*() != L'\0');
}

bool Utf8ToUtf16Iterator::operator==(const Utf8ToUtf16Iterator &rhs) const
{
    return (!rhs.m_str && !operator*()) || (!m_str && !rhs.operator*()) ||
        (m_str == rhs.m_str && m_len == rhs.m_len && m_pos == rhs.m_pos && m_subPos == rhs.m_subPos);
}

bool Utf8ToUtf16Iterator::operator!=(const Utf8ToUtf16Iterator &rhs) const
{
    return !operator==(rhs);
}

Utf8ToUtf16Iterator &Utf8ToUtf16Iterator::operator++()
{
    if (m_str)
    {
        uint32_t codepoint = 0;
        uint8_t offset = 1;
        if (m_len == -1)
        {
            codepoint = Utf8SequenceToUnicodeCodepoint(m_str + m_pos, 4);
            offset = GetUtf8SequenceLength(m_str + m_pos, 4);
        }
        else if (m_pos < m_len)
        {
            size_t remainder = m_len - m_pos;
            codepoint = Utf8SequenceToUnicodeCodepoint(m_str + m_pos, remainder);
            offset = GetUtf8SequenceLength(m_str + m_pos, remainder);
        }

        uint8_t length = GetUtf16SequenceLength(codepoint);
        ++m_subPos;
        if (m_subPos >= length)
        {
            m_subPos = 0;
            m_pos += offset;
            // TODO check for buffer overflow
        }
    }

    return *this;
}

Utf8ToUtf16Iterator Utf8ToUtf16Iterator::operator++(int)
{
    Utf8ToUtf16Iterator temp(*this);
    operator++();
    return temp;
}

utf16char_t Utf8ToUtf16Iterator::operator*() const
{
    utf16char_t unit = 0;
    if (m_str && (m_len == -1 || (m_pos >= 0 && m_pos < m_len)))
    {
        size_t remainder = m_len - m_pos;
        uint32_t codepoint = Utf8SequenceToUnicodeCodepoint(m_str + m_pos, remainder);
        volatile uint32_t sequence = UnicodeCodepointToUtf16Sequence(codepoint);
        unit = ((utf16char_t *)&sequence)[m_subPos];
    }

    return unit;
}
