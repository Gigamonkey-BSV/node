cmake_minimum_required (VERSION 3.16)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CPP_COMPILER "g++")

project (node CXX)

set (CONAN_DISABLE_CHECK_COMPILER ON)

# Set cmake as import path for Find*.cmake files
include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup (TARGETS)

find_package (laserpants_dotenv)
add_executable (node src/node.cpp src/program_options.cpp)
target_link_libraries (node PUBLIC
#  CONAN_PKG::Gigamonkey
  laserpants::dotenv
  CONAN_PKG::nlohmann_json
  CONAN_PKG::argh
  CONAN_PKG::libpqxx
  CONAN_PKG::boost)
target_include_directories (node PUBLIC include)

target_compile_features (node PUBLIC cxx_std_20)
set_target_properties (node PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (node PUBLIC "-fconcepts")
