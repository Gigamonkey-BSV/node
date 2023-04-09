conan install . --output-folder=build --build=missing
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/home/d/code/Gigamonkey/node/build/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
