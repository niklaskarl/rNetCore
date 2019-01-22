cmake_minimum_required (VERSION 2.8.11)
project (clrhost_test)

add_executable (clrhost_test
    main.cpp
    )

add_test (TEST clrhost_test COMMAND clrhost_test)

target_link_libraries (clrhost_test gtest_main clrhost)
