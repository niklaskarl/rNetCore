cmake_minimum_required (VERSION 2.8.11)
project (rNetCore)

set (CMAKE_CXX_STANDARD 14)

enable_testing()

add_subdirectory (./package/src/semver)
add_subdirectory (./package/src/stringtranscoding)
add_subdirectory (./package/src/pal)
add_subdirectory (./package/src/json)
add_subdirectory (./package/src/clrhost)

add_subdirectory (./test)
