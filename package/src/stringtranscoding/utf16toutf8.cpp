#include "utf16toutf8.h"

uint32_t Utf16SequenceToUnicodeCodepoint(const utf16char_t *sequence, const size_t &maxLength)
{
    uint32_t codepoint = 0;
    if (maxLength >= 2 &&
        (sequence[0] & 0xFC00) == 0xD800 &&
        (sequence[1] & 0xFC00) == 0xDC00)
    {
        codepoint = 0x10000 + (((sequence[0] & 0x03FF) << 10) | (sequence[1] & 0x03FF));
    }
    else if (maxLength >= 1)
    {
        codepoint = *sequence;
    }

    return codepoint;
}

uint8_t GetUtf16SequenceLength(const utf16char_t *sequence, const size_t &maxLength)
{
    uint8_t length = 1;
    if (maxLength >= 2 &&
        (sequence[0] & 0xFC00) == 0xD800 &&
        (sequence[1] & 0xFC00) == 0xDC00)
    {
        length = 2;
    }
    else if (maxLength >= 1)
    {
        length = 1;
    }

    return length;
}

uint8_t GetUtf8SequenceLength(uint32_t codepoint)
{
    uint8_t length = 0;
    if (codepoint < 0x0080)
    {
        length = 1;
    }
    else if (codepoint < 0x0800)
    {
        length = 2;
    }
    else if (codepoint < 0x10000)
    {
        length = 3;
    }
    else if (codepoint < 0x200000)
    {
        length = 4;
    }

    return length;
}

uint32_t UnicodeCodepointToUtf8Sequence(const uint32_t &codepoint)
{
    volatile uint32_t sequence = 0;
    uint8_t *units = (uint8_t *)&sequence;

    if (codepoint < 0x0080)
    {
        units[0] = (uint8_t)codepoint;
    }
    else if (codepoint < 0x0800)
    {
        units[0] = 0b11000000 | ((codepoint >> 6) & 0b00011111);
        units[1] = 0b10000000 | ((codepoint >> 0) & 0b00111111);
    }
    else if (codepoint < 0x10000)
    {
        units[0] = 0b11100000 | ((codepoint >> 12) & 0b00001111);
        units[1] = 0b10000000 | ((codepoint >>  6) & 0b00111111);
        units[2] = 0b10000000 | ((codepoint >>  0) & 0b00111111);
    }
    else if (codepoint < 0x200000)
    {
        units[0] = 0b11110000 | ((codepoint >> 18) & 0b00000111);
        units[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
        units[2] = 0b10000000 | ((codepoint >>  6) & 0b00111111);
        units[3] = 0b10000000 | ((codepoint >>  0) & 0b00111111);
    }

    return sequence;
}

Utf16ToUtf8Iterator::Utf16ToUtf8Iterator(const utf16char_t *str, ptrdiff_t len, ptrdiff_t pos) :
    m_str(str),
    m_len(len),
    m_pos(pos),
    m_subPos(0)
{ }

Utf16ToUtf8Iterator::Utf16ToUtf8Iterator(const Utf16ToUtf8Iterator& iter) :
    m_str(iter.m_str),
    m_len(iter.m_len),
    m_pos(iter.m_pos),
    m_subPos(iter.m_subPos)
{ }

Utf16ToUtf8Iterator::operator bool() const
{
    return m_str && (m_len >= 0 ? m_pos < m_len : operator*() != '\0');
}

bool Utf16ToUtf8Iterator::operator==(const Utf16ToUtf8Iterator &rhs) const
{
    return (!rhs.m_str && !operator*()) || (!m_str && !rhs.operator*()) ||
        (m_str == rhs.m_str && m_len == rhs.m_len && m_pos == rhs.m_pos && m_subPos == rhs.m_subPos);
}

bool Utf16ToUtf8Iterator::operator!=(const Utf16ToUtf8Iterator &rhs) const
{
    return !operator==(rhs);
}

Utf16ToUtf8Iterator &Utf16ToUtf8Iterator::operator++()
{
    if (m_str)
    {
        uint32_t codepoint = 0;
        uint8_t offset = 1;
        if (m_len == -1)
        {
            codepoint = Utf16SequenceToUnicodeCodepoint(m_str + m_pos, 2);
            offset = GetUtf16SequenceLength(m_str + m_pos, 2);
        }
        if (m_pos < m_len)
        {
            size_t remainder = m_len - m_pos;
            codepoint = Utf16SequenceToUnicodeCodepoint(m_str + m_pos, remainder);
            offset = GetUtf16SequenceLength(m_str + m_pos, remainder);
        }

        uint8_t length = GetUtf8SequenceLength(codepoint);
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

Utf16ToUtf8Iterator Utf16ToUtf8Iterator::operator++(int)
{
    Utf16ToUtf8Iterator temp(*this);
    operator++();
    return temp;
}

utf8char_t Utf16ToUtf8Iterator::operator*() const
{
    utf8char_t unit = 0;
    if (m_str && (m_len == -1 || (m_pos >= 0 && m_pos < m_len)))
    {
        size_t remainder = m_len - m_pos;
        uint32_t codepoint = Utf16SequenceToUnicodeCodepoint(m_str + m_pos, remainder);
        volatile uint32_t sequence = UnicodeCodepointToUtf8Sequence(codepoint);
        unit = ((utf8char_t *)&sequence)[m_subPos];
    }

    return unit;
}
