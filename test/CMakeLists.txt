cmake_minimum_required (VERSION 2.8.11)
project (Tests)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(./gtest)
add_subdirectory (./gtest)
add_subdirectory (./semver)
add_subdirectory (./stringtranscoding)
add_subdirectory (./clrhost)
