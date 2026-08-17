cmake_minimum_required(VERSION 3.14)   

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lib (part 1)
set(LOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileLogTarget.cpp
)

find_package(Threads REQUIRED)

add_library(logger_static STATIC ${LOGGER_SOURCES})
target_include_directories(logger_static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/include)
target_link_libraries(logger_static PUBLIC Threads::Threads)
set_target_properties(logger_static PROPERTIES
    OUTPUT_NAME logger
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static)

add_library(logger_shared SHARED ${LOGGER_SOURCES})
target_include_directories(logger_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/include)
target_link_libraries(logger_shared PUBLIC Threads::Threads)
set_target_properties(logger_shared PROPERTIES
    OUTPUT_NAME logger
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared)

# examples
add_executable(example        ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
target_link_libraries(example PRIVATE logger_shared)

add_executable(example_static ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
target_link_libraries(example_static PRIVATE logger_static)

# multi thread app
# ---- Приложение (Часть 2) ----
add_executable(logger_app app/main.cpp)
target_include_directories(logger_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/app)
target_link_libraries(logger_app PRIVATE
    logger_static
    Threads::Threads)
    
# gtest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(logger_test tests/test_logger.cpp)
target_link_libraries(logger_test PRIVATE logger_static GTest::gtest_main)

add_test(NAME logger_test COMMAND logger_test)