cmake_minimum_required (VERSION 2.8.11)
project (semver)

add_library (semver STATIC
    semanticversion.cpp
    )
