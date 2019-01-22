cmake_minimum_required (VERSION 2.8.11)
project (clrhost)

add_library (clrhost STATIC
    clrhosting.cpp
    clrhostwrapper.cpp
    appdomainwrapper.cpp
    dotnet/appdomain_dotnet.cpp
    dotnet/win/clrhost_dotnet_win.cpp
    dotnet/unix/clrhost_dotnet_unix.cpp
    )

target_link_libraries (clrhost semver stringtranscoding pal)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o -fshort-wchar")
endif(UNIX)
