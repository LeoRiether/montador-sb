cmake_minimum_required(VERSION 3.10)
project(MONTADOR)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add main executable
add_compile_options(-Wall -Wextra -Werror=init-self)
add_executable(MONTADOR ${SOURCES})

# We're using Catch2 as a testing library
# https://github.com/catchorg/Catch2
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
file(GLOB TESTS_SOURCES "tests/*.cpp")
file(GLOB AAAAA "src/*.cpp")
list(FILTER AAAAA EXCLUDE REGEX "src/main.cpp$")
add_executable(tests ${TESTS_SOURCES} ${AAAAA})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
