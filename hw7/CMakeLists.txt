cmake_minimum_required(VERSION 4.4)

project(TestContainersGTest)

# build GTests
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest-1.18.0"
    "googletest"
)

## test targets
add_executable(
    list_tests 
    list_tests.cpp
)

add_executable(
    vector_tests
    vector_tests.cpp
)

add_executable(
    additional_1_tests
    additional_1_tests.cpp
)

add_executable(
    additional_3_tests
    additional_3_tests.cpp
)

# add src to the include directories for the test target
target_include_directories(
    list_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_include_directories(
    vector_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_include_directories(
    additional_1_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_include_directories(
    additional_3_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# link test target against gtest libraries
target_link_libraries(
    list_tests PRIVATE
    gtest gtest_main
)

target_link_libraries(
    vector_tests PRIVATE
    gtest gtest_main
)

target_link_libraries(
    additional_1_tests PRIVATE
    gtest gtest_main
)

target_link_libraries(
    additional_3_tests PRIVATE
    gtest gtest_main
)

if (MSVC)
    # @see https://github.com/google/googletest/blob/master/googletest/README.md
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(
        list_tests PRIVATE
        /W4
    )
    target_compile_options(
        vector_tests PRIVATE
        /W4
    )

    target_compile_options(
        additional_1_tests PRIVATE
        /W4
    )

    target_compile_options(
        additional_3_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(
        list_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
        vector_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    target_compile_options(
        additional_1_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    target_compile_options(
        additional_3_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()