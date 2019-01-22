cmake_minimum_required (VERSION 2.8.11)
project (pal)

add_library (pal STATIC
    path.cpp
    path.unix.cpp
    path.win.cpp
    linking.unix.cpp
    linking.win.cpp
    )

if (WIN32)
    find_library(SHLWAPI shlwapi.lib)
    target_link_libraries (pal SHLWAPI)

    target_link_libraries (pal stringtranscoding)
endif(WIN32)

if (UNIX)
    target_link_libraries (pal dl)
endif(UNIX)
