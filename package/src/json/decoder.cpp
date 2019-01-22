#include "json.h"

int8_t ToHex(char c)
{
    int8_t result = -1;
    if (c >= '0' && c <= '9')
    {
        result = c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        result = c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        result = c - 'A' + 10;
    }

    return result;
}

uint8_t GetUtf8Length(uint32_t codepoint)
{
    uint8_t result = 0;
    if (codepoint < 0x0080)
    {
        result = 1;
    }
    else if (codepoint < 0x0800)
    {
        result = 2;
    }
    else if (codepoint < 0x10000)
    {
        result = 3;
    }
    else if (codepoint < 0x200000)
    {
        result = 4;
    }

    return result;
}

uint32_t GetUtf8Sequence(uint16_t codepoint)
{
    uint32_t sequence = 0;
    if (codepoint < 0x0080)
    {
        sequence = codepoint;
    }
    else if (codepoint < 0x0800)
    {
        sequence =               0b1100000010000000 |
            (codepoint &         0b0000000000111111) |
            ((codepoint << 2) &  0b1111100000000);

        sequence <<= 16;
    }
    else if (codepoint < 0x10000)
    {
        sequence =              0b111000001000000010000000 |
            (codepoint &        0b000000000000000000111111) |
            ((codepoint << 2) & 0b0000000000011111100000000) |
            ((codepoint << 4) & 0b0000011110000000000000000);

        sequence <<= 8;
    }
    else if (codepoint < 0x200000)
    {
        sequence =              0b11110000100000001000000010000000 |
            (codepoint &        0b00000000000000000000000000111111) |
            ((codepoint << 2) & 0b00000000000000000011111100000000) |
            ((codepoint << 4) & 0b00000000001111110000000000000000) |
            ((codepoint << 6) & 0b00000111000000000000000000000000);
    }

    return sequence;
}

Json::JStringIterator::JStringIterator(const char *str, size_t len, size_t pos) :
    m_str(str),
    m_len(len),
    m_pos(pos),
    m_byte(0)
{
}

Json::JStringIterator::JStringIterator(const JStringIterator& dec) :
    m_str(dec.m_str),
    m_len(dec.m_len),
    m_pos(dec.m_pos),
    m_byte(dec.m_byte)
{
}

Json::JStringIterator& Json::JStringIterator::operator++()
{
    if (m_pos >= 0 && m_pos < m_len && m_str[m_pos] == '\\')
    {
        ++m_pos;
        if (m_pos < m_len)
        {
            switch (m_str[m_pos])
            {
                case '\"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    ++m_pos;
                    break;
                case 'u':
                    if (m_pos + 4 < m_len)
                    {
                        int8_t h1 = ToHex(m_str[m_pos + 1]);
                        int8_t h2 = ToHex(m_str[m_pos + 2]);
                        int8_t h3 = ToHex(m_str[m_pos + 3]);
                        int8_t h4 = ToHex(m_str[m_pos + 4]);
                        if (h1 >= 0 && h2 >= 0 && h3 >= 0 && h4 >= 0)
                        {
                            uint16_t codepoint = h1 << 12 | h2 << 8 | h3 << 4 | h4;
                            uint8_t length = GetUtf8Length(codepoint);
                            if (m_byte < length - 1)
                            {
                                --m_pos;
                                ++m_byte;
                            }
                            else
                            {
                                m_byte = 0;
                                m_pos += 5;
                            }
                        }
                    }

                    break;
            }
        }
    }
    else
    {
        ++m_pos;
    }

    return *this;
}

Json::JStringIterator Json::JStringIterator::operator++(int)
{
    JStringIterator tmp(*this);
    operator++();
    return tmp;
}

bool Json::JStringIterator::operator==(const Json::JStringIterator& rhs) const
{
    return m_str == rhs.m_str && m_len == rhs.m_len && m_pos == rhs.m_pos && m_byte == rhs.m_byte;
}

bool Json::JStringIterator::operator!=(const Json::JStringIterator& rhs) const
{
    return !operator==(rhs);
}

char Json::JStringIterator::operator*()
{
    char result = '\0';
    if (m_pos >= 0 && m_pos < m_len)
    {
        result = m_str[m_pos];
        if (result == '\\')
        {
            if (m_pos + 1 < m_len)
            {
                switch (m_str[m_pos + 1])
                {
                    case '\"':
                        result = '\"';
                        break;
                    case '\\':
                        result = '\\';
                        break;
                    case '/':
                        result = '/';
                        break;
                    case 'b':
                        result = '\b';
                        break;
                    case 'f':
                        result = '\f';
                        break;
                    case 'n':
                        result = '\n';
                        break;
                    case 'r':
                        result = '\r';
                        break;
                    case 't':
                        result = '\t';
                        break;
                    case 'u':
                        if (m_pos + 5 < m_len)
                        {
                            int8_t h1 = ToHex(m_str[m_pos + 2]);
                            int8_t h2 = ToHex(m_str[m_pos + 3]);
                            int8_t h3 = ToHex(m_str[m_pos + 4]);
                            int8_t h4 = ToHex(m_str[m_pos + 5]);
                            if (h1 >= 0 && h2 >= 0 && h3 >= 0 && h4 >= 0)
                            {
                                uint16_t codepoint = h1 << 12 | h2 << 8 | h3 << 4 | h4;
                                uint32_t sequence = GetUtf8Sequence(codepoint);

                                if (m_byte < 4)
                                {
                                    result = ((char *)&sequence)[3 - m_byte];
                                }
                            }
                        }

                        break;
                }
            }
        }
    }

    return result;
}
