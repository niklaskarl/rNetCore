cmake_minimum_required (VERSION 2.8.11)
project (stringtranscoding_test)

add_executable (stringtranscoding_test
    main.cpp
    )

add_test (TEST stringtranscoding_test COMMAND stringtranscoding_test)

target_link_libraries (stringtranscoding_test gtest_main)
