#include "semanticversion.h"

#include <utility>

bool ParseNumber(const char **text, uint32_t *result)
{
    uint32_t acc;
    const char *number = *text;
    if (*number == '0')
    {
        ++number;
        *text = number;
        *result = 0;
        return true;
    }
    else if (*number >= '1' && *number <= '9')
    {
        acc = *number - '0';
        ++number;
        while (*number >= '0' && *number <= '9')
        {
            acc *= 10;
            acc += *number - '0';
            ++number;
        }

        *result = acc;
        *text = number;
        return true;
    }

    *result = 0;
    return false;
}

void PrintNumberRecursive(uint32_t number, std::string &dest)
{
    if (number > 0)
    {
        PrintNumberRecursive(number / 10, dest);
        dest.append(1, (char)('0' + (number % 10)));
    }
}

void PrintNumber(uint32_t number, std::string &dest)
{
    if (number > 0)
    {
        PrintNumberRecursive(number, dest);
    }
    else
    {
        dest.append("0");
    }
}

int32_t ComparePreReleaseIdentifier(const char **lhs, const char **rhs)
{
    const char *lhsIter = *lhs;
    const char *rhsIter = *rhs;
    uint32_t lhsNumber;
    uint32_t rhsNumber;
    bool lhsIsNumber = ParseNumber(&lhsIter, &lhsNumber);
    bool rhsIsNumber = ParseNumber(&rhsIter, &rhsNumber);
    lhsIsNumber = lhsIsNumber && (*lhsIter == '.' || *lhsIter == '\0');
    rhsIsNumber = rhsIsNumber && (*rhsIter == '.' || *rhsIter == '\0');

    int32_t result = 0;
    if (lhsIsNumber && rhsIsNumber)
    {
        *lhs = lhsIter;
        *rhs = rhsIter;

        result = rhsNumber - lhsNumber;
    }
    else if (lhsIsNumber && !rhsIsNumber)
    {
        // rhs is not a number, so it is greater than lhs
        // rhs > lhs
        result = 1;
    }
    else if (!lhsIsNumber && rhsIsNumber)
    {
        // lhs is not a number, so it is greater than rhs
        // lhs > rhs
        result = -1;
    }
    else
    {
        // compare lexically
        lhsIter = *lhs;
        rhsIter = *rhs;

        char lhsChar = *lhsIter;
        char rhsChar = *rhsIter;
        while (result == 0 && lhsChar != '\0' && lhsChar != '.' && rhsChar != '\0' && rhsChar != '.')
        {
            result = rhsChar - lhsChar;

            lhsChar = *(++lhsIter);
            rhsChar = *(++rhsIter);
        }

        if (result == 0 && lhsChar != '\0' && lhsChar != '.' && (rhsChar == '\0' || rhsChar == '.'))
        {
            // rhs is shorter than lhs
            return 1;
        }
        else if (result == 0 && (lhsChar == '\0' || lhsChar == '.') && rhsChar != '\0' && rhsChar != '.')
        {
            // lhs is shorter than rhs
            return -1;
        }

        *lhs = lhsIter;
        *rhs = rhsIter;
    }

    return result;
}

int32_t ComparePreReleaseLabels(const char *lhs, const char *rhs)
{
    int32_t result = 0;
    if (*lhs == '\0' && *rhs != '\0')
    {
        result = -1;
    }
    else if (*lhs != '\0' && *rhs == '\0')
    {
        result = 1;
    }
    else
    {
        bool done = false;
        while (!done)
        {
            if (*lhs == '\0' && *rhs != '\0')
            {
                done = true;
                result = 1;
            }
            else if (*lhs != '\0' && *rhs == '\0')
            {
                done = true;
                result = -1;
            }
            else if (*lhs == '\0' && *rhs == '\0')
            {
                done = true;
                result = 0;
            }
            else
            {
                result = ComparePreReleaseIdentifier(&lhs, &rhs);
                if (result != 0)
                {
                    done = true;
                }
                else
                {
                    if (*lhs == '.')
                    {
                        ++lhs;
                    }

                    if (*rhs == '.')
                    {
                        ++rhs;
                    }
                }
            }
        }
    }

    return result;
}

SemVer::SemVer(uint32_t major, uint32_t minor, uint32_t revision, const char *preRelease, const char *buildMetadata) noexcept :
    m_major(major),
    m_minor(minor),
    m_patch(revision),
    m_preRelease(),
    m_buildMetadata()
{
    if (preRelease)
    {
        m_preRelease.assign(preRelease);
    }

    if (buildMetadata)
    {
        m_buildMetadata.assign(buildMetadata);
    }
}

SemVer::SemVer(const SemVer &version) noexcept :
    m_major(version.m_major),
    m_minor(version.m_minor),
    m_patch(version.m_patch),
    m_preRelease(version.m_preRelease),
    m_buildMetadata(version.m_buildMetadata)
{ }

#ifdef __HAS_MOVE_CONSTRUCTOR__
SemVer::SemVer(SemVer &&version) noexcept :
    m_major(std::exchange(version.m_major, 0)),
    m_minor(std::exchange(version.m_minor, 0)),
    m_patch(std::exchange(version.m_patch, 0)),
    m_preRelease(std::move(version.m_preRelease)),
    m_buildMetadata(std::move(version.m_buildMetadata))
{ }
#endif // __HAS_MOVE_CONSTRUCTOR__

SemVer &SemVer::operator=(const SemVer &version) noexcept
{
    m_major = version.m_major;
    m_minor = version.m_minor;
    m_patch = version.m_patch;
    m_preRelease = version.m_preRelease;
    m_buildMetadata = version.m_buildMetadata;

    return *this;
}

#ifdef __HAS_MOVE_ASSIGNMENT__
SemVer &SemVer::operator=(SemVer &&version) noexcept
{
    std::swap(m_major, version.m_major);
    std::swap(m_minor, version.m_minor);
    std::swap(m_patch, version.m_patch);
    m_preRelease.swap(version.m_preRelease);
    m_buildMetadata.swap(version.m_buildMetadata);

    return *this;
}
#endif // __HAS_MOVE_ASSIGNMENT__

uint32_t SemVer::GetMajor() const noexcept
{
    return m_major;
}

uint32_t SemVer::GetMinor() const noexcept
{
    return m_minor;
}

uint32_t SemVer::GetPatch() const noexcept
{
    return m_patch;
}

const std::string &SemVer::GetPreRelease() const noexcept
{
    return m_preRelease;
}

const std::string &SemVer::GetBuildMetadata() const noexcept
{
    return m_buildMetadata;
}

bool SemVer::operator==(const SemVer &rhs) const noexcept
{
    return m_major == rhs.m_major && m_minor == rhs.m_minor && m_patch == rhs.m_patch
        && m_preRelease.compare(rhs.m_preRelease) == 0;
}

bool SemVer::operator!=(const SemVer &rhs) const noexcept
{
    return !operator==(rhs);
}

bool SemVer::operator<(const SemVer &rhs) const noexcept
{
    return m_major < rhs.m_major ||
        (m_major == rhs.m_major && (m_minor < rhs.m_minor ||
        (m_minor == rhs.m_minor && (m_patch < rhs.m_patch ||
        (m_patch == rhs.m_patch && ComparePreReleaseLabels(m_preRelease.c_str(), rhs.m_preRelease.c_str()) > 0)))));
}

bool SemVer::operator<=(const SemVer &rhs) const noexcept
{
    return operator<(rhs) || operator==(rhs);
}

bool SemVer::operator>(const SemVer &rhs) const noexcept
{
    return m_major > rhs.m_major ||
        (m_major == rhs.m_major && (m_minor > rhs.m_minor ||
        (m_minor == rhs.m_minor && (m_patch > rhs.m_patch ||
        (m_patch == rhs.m_patch && ComparePreReleaseLabels(m_preRelease.c_str(), rhs.m_preRelease.c_str()) < 0)))));
}

bool SemVer::operator>=(const SemVer &rhs) const noexcept
{
    return operator>(rhs) || operator==(rhs);
}

std::string SemVer::ToString() const noexcept
{
    std::string result;

    PrintNumber(m_major, result);
    result.append(".");
    PrintNumber(m_minor, result);
    result.append(".");
    PrintNumber(m_patch, result);

    if (m_preRelease.length())
    {
        result.append("-");
        result.append(m_preRelease);
    }

    if (m_buildMetadata.length())
    {
        result.append("+");
        result.append(m_buildMetadata);
    }

    return result;
}

bool SemVer::TryParse(const char *text, SemVer &version) noexcept
{
    uint32_t major;
    uint32_t minor;
    uint32_t revision;
    const char *preRelease = nullptr;
    size_t preReleaseLength = 0;
    const char *buildMetadata = nullptr;
    size_t buildMetadataLength = 0;

    bool success = false;
    if (ParseNumber(&text, &major))
    {
        if (*text == '.')
        {
            ++text;
            if (ParseNumber(&text, &minor))
            {
                if (*text == '.')
                {
                    ++text;
                    if (ParseNumber(&text, &revision))
                    {
                        success = true;

                        char c = *text;
                        if (c == '-')
                        {
                            ++text;
                            const char *end = text;
                            c = *end;
                            while ((c >= 'a' && c <= 'z') ||
                                (c >= 'A' && c <= 'Z') ||
                                (c >= '0' && c <= '9') ||
                                c == '.' || c == '-')
                            {
                                c = *(++end);
                            }

                            if (end != text)
                            {
                                preRelease = text;
                                preReleaseLength = end - text;
                                text = end;
                            }
                            else
                            {
                                success = false;
                            }
                        }

                        if (success && c == '+')
                        {
                            ++text;
                            const char *end = text;
                            c = *end;
                            while ((c >= 'a' && c <= 'z') ||
                                (c >= 'A' && c <= 'Z') ||
                                (c >= '0' && c <= '9'))
                            {
                                c = *(++end);
                            }

                            if (end != text)
                            {
                                buildMetadata = text;
                                buildMetadataLength = end - text;
                                text = end;
                            }
                            else
                            {
                                success = false;
                            }
                        }

                        if (success && c == '\0')
                        {
                            version.m_major = major;
                            version.m_minor = minor;
                            version.m_patch = revision;
                            version.m_preRelease = std::string(preRelease, preReleaseLength);
                            version.m_buildMetadata = std::string(buildMetadata, buildMetadataLength);
                        }
                    }
                }
            }
        }
    }

    return success;
}
