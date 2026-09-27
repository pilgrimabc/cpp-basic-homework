cmake_minimum_required(VERSION 4.4.1)
project(tencon-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#######################################################
## add external libs via FetchContent
#######################################################
include(FetchContent)

# xtl 
FetchContent_Declare(
    xtl
    GIT_REPOSITORY https://github.com/xtensor-stack/xtl.git
    GIT_TAG 0.8.0
)

FetchContent_MakeAvailable(xtl)

#xtensor
FetchContent_Declare(
    xtensor
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor.git
    GIT_TAG 0.27.0
)

FetchContent_MakeAvailable(xtensor)

#############################################
## add my tensor contraction (TenCon) lib
#############################################

add_library(tencon
    src/tensor.cpp
    src/tensor_network.cpp
    src/contraction.cpp
    src/optimizer.cpp
)

target_include_directories(tencon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${xtensor_SOURCE_DIR}/include
    PUBLIC ${xtl_SOURCE_DIR}/include
)

target_link_libraries(tencon
    PRIVATE xtensor
)

### example 1

add_executable(example_1_tensor
    examples/exaple_1_tensor.cpp
)

target_link_libraries(example_1_tensor
    PRIVATE tencon
)

### example 2

add_executable(example_2_contraction
    examples/example_2_contraction.cpp
)

target_link_libraries(example_2_contraction
    PRIVATE tencon
)

### example 3

add_executable(example_3_tensor_network
    examples/example_3_tensor_network.cpp
)

target_link_libraries(example_3_tensor_network
    PRIVATE tencon
)

### example 4

add_executable(example_4_optimizer
    examples/example_4_optimizer.cpp
)

target_link_libraries(example_4_optimizer
    PRIVATE tencon
)

### example 5

add_executable(example_5_full_pipeline
    examples/example_5_full_pipeline.cpp
)

target_link_libraries(example_5_full_pipeline
    PRIVATE tencon
)
