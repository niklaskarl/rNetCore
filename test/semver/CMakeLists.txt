cmake_minimum_required (VERSION 2.8.11)
project (semver_test)

add_executable (semver_test
    main.cpp
    )

add_test (TEST semver_test COMMAND semver_test)

target_link_libraries (semver_test gtest_main semver)
