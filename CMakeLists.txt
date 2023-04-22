cmake_minimum_required (VERSION 3.16)

set (CMAKE_C_COMPILER "gcc")
set (CMAKE_CPP_COMPILER "g++")

project (node CXX)

set (CONAN_DISABLE_CHECK_COMPILER ON)

find_package (Boost CONFIG REQUIRED)
find_package (data CONFIG REQUIRED)
find_package (pegtl CONFIG REQUIRED)
find_package (argh CONFIG REQUIRED)
find_package (libpqxx CONFIG REQUIRED)
find_package (nlohmann_json CONFIG REQUIRED)

add_definitions ("-DHAS_BOOST")

add_executable (node
  src/node.cpp
  src/calc.cpp
  src/postgres.cpp
  src/program_options.cpp)

target_link_libraries (node PUBLIC
  argh
  nlohmann_json::nlohmann_json
  taocpp::pegtl
  pqxx
  boost::boost
  data::data)

target_include_directories (node PUBLIC include)

target_compile_features (node PUBLIC cxx_std_20)
set_target_properties (node PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (node PUBLIC "-fconcepts")
