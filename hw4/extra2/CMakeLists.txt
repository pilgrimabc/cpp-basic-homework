cmake_minimum_required(VERSION 3.10)
project(physics)

set(CMAKE_CXX_STANDARD 17)

# Явно указываем CMake, где искать старую версию SFML 2.6 из Homebrew
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

# Ищем системный SFML 2.6 (компоненты должны быть с маленькой буквы, как в исходном коде задания)
find_package(SFML COMPONENTS graphics window system REQUIRED)

if (SFML_FOUND)
    message(STATUS "SUCCESS: Found SFML 2.6.x via Homebrew!")
endif ()

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

add_executable(physics
        main.cpp World.cpp Ball.cpp Physics.cpp Color.cpp Painter.cpp
        black_box/Application.cpp black_box/View.cpp black_box/PainterImpl.cpp)

# Используем оригинальный таргет из задания OTUS
target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )
