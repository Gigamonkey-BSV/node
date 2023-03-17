cmake_minimum_required (VERSION 3.16)

# Set cmake as import path for Find*.cmake files
include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup (TARGETS)

project (node)

target_include_directories (node PUBLIC include)

find_package (laserpants_dotenv)
add_executable (node src/node.cpp)
target_link_libraries (node
  laserpants::dotenv
  CONAN_PKG::nlohmann_json
  CONAN_PKG::argh
  CONAN_PKG::pqxx
  CONAN_PKG::boost)

target_compile_features (node PUBLIC cxx_std_20)
set_target_properties (node PROPERTIES CXX_EXTENSIONS ON)
target_compile_options (node PUBLIC "-fconcepts")
