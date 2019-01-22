#ifndef __SEMANTICVERSION_H__
#define __SEMANTICVERSION_H__

#include <inttypes.h>
#include <string>

class SemVer
{
public:
    SemVer(uint32_t major = 0, uint32_t minor = 0, uint32_t revision = 0, const char *preRelease = NULL, const char *buildMetadata = NULL) noexcept;
    SemVer(const SemVer &version) noexcept;

#ifdef __HAS_MOVE_CONSTRUCTOR__
    SemVer(SemVer &&version) noexcept;
#endif // __HAS_MOVE_CONSTRUCTOR__

    SemVer &operator=(const SemVer &version) noexcept;

#ifdef __HAS_MOVE_ASSIGNMENT__
    SemVer &operator=(SemVer &&version) noexcept;
#endif // __HAS_MOVE_ASSIGNMENT__

    uint32_t GetMajor() const noexcept;
    uint32_t GetMinor() const noexcept;
    uint32_t GetPatch() const noexcept;
    const std::string &GetPreRelease() const noexcept;
    const std::string &GetBuildMetadata() const noexcept;

    bool operator==(const SemVer &rhs) const noexcept;
    bool operator!=(const SemVer &rhs) const noexcept;

    bool operator<(const SemVer &rhs) const noexcept;
    bool operator<=(const SemVer &rhs) const noexcept;
    bool operator>(const SemVer &rhs) const noexcept;
    bool operator>=(const SemVer &rhs) const noexcept;

    std::string ToString() const noexcept;

    static bool TryParse(const char *text, SemVer &version) noexcept;

private:
    uint32_t m_major;
    uint32_t m_minor;
    uint32_t m_patch;
    std::string m_preRelease;
    std::string m_buildMetadata;
};

#endif // __SEMANTICVERSION_H__
