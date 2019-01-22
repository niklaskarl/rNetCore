cmake_minimum_required (VERSION 2.8.11)
project (json)

add_library (json STATIC
    jvalue.cpp
    jtoken.cpp
    jobject.cpp
    jarray.cpp
    jstring.cpp
    decoder.cpp
    parser.cpp
    )
