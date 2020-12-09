#!/bin/sh

flags=""
testOn=0

for param in "$@"
do
    # Calling "./build.sh clean" => Clean folder
    if [ "$param" = "clean" ]; then
        rm -rf build 2>/dev/null
        rm -rf bin 2>/dev/null
        echo "Folders cleaned."
        exit
    fi
    # Calling "./build.sh test" or "./build.sh dev" => tests
    if [ "$param" = "dev" ] || [ "$param" = "test" ]; then
        flags="${flags} -D BUILD_TESTS=ON"
        testOn=1
    fi

    # Calling "./build.sh test" disable library build
    if [ "$param" = "test" ]; then
        flags="${flags} -D BUILD_LIBS=OFF"
    fi
done

mkdir -p build
mkdir -p bin
cp -r tests/unit/data build
cd build
cmake $flags ..
cmake --build . -j8

# Compilation has terminated successfully 
if [ $? -eq 0 ]; then
    mv libcpphttp* ../bin 2>/dev/null
    if [ $testOn -eq 1 ]; then
        mv unittest ../bin 2>/dev/null
        ../bin/unittest
    fi
fi
