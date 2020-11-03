#!/bin/sh

flags=""
dev=0

for param in "$@"
do
    # Calling "./build.sh clean" => Clean folder
    if [ "$param" = "clean" ]; then
        rm -rf build 2>/dev/null
        rm -rf bin 2>/dev/null
        echo "Folders cleaned."
        exit
    fi
    # Calling "./build.sh dev" => debug + tests
    if [ "$param" = "dev" ]; then
        flags="-D DEVELOPMENT_BUILD=ON"
        dev=1
    fi
done

mkdir -p build
mkdir -p bin
cd build
cmake $flags ..
cmake --build . 

# Compilation has terminated successfully 
if [ $? -eq 0 ]; then
    mv project ../bin
    if [ $dev -eq 1 ]; then
        mv unittest ../bin
        ../bin/unittest
    fi
fi
