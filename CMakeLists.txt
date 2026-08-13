cmake_minimum_required(VERSION 3.10)
project(logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы библиотеки
set(LOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileLogTarget.cpp
)

# Статическая библиотека
add_library(logger_static STATIC ${LOGGER_SOURCES})
target_include_directories(logger_static PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/include  # ✅ Правильный путь!
)
set_target_properties(logger_static PROPERTIES OUTPUT_NAME logger)

# Динамическая библиотека
add_library(logger_shared SHARED ${LOGGER_SOURCES})
target_include_directories(logger_shared PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/include  # ✅ Правильный путь!
)
set_target_properties(logger_shared PROPERTIES OUTPUT_NAME logger)

# Подключаем Threads
find_package(Threads REQUIRED)
target_link_libraries(logger_static PUBLIC Threads::Threads)
target_link_libraries(logger_shared PUBLIC Threads::Threads)

# Пример с динамической библиотекой
add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
target_link_libraries(example PRIVATE logger_shared)
add_dependencies(example logger_shared) 

# Пример со статической библиотекой
add_executable(example_static ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
target_link_libraries(example_static PRIVATE logger_static)
add_dependencies(example_static logger_static)



include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(logger_test
    tests/test_logger.cpp  
)

target_link_libraries(logger_test
    PRIVATE
    logger_static
    GTest::gtest_main
)

target_include_directories(logger_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/include
)

add_test(NAME logger_test COMMAND logger_test)