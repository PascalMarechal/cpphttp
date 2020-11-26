# Cpp Http
Do HTTP with this static or dynamic linked library.\
It is mainly inspired by node + express and wants to be easy to use.

## Before You Begin
You need to install cmake (with C++17 compiler) and asio to build the main library.\
For example on Ubuntu/Debian you can do :
```bash
# cmake & g++ compiler
sudo apt update -y && sudo apt-get install -y build-essential cmake wget
# asio
wget -qO- "https://sourceforge.net/projects/asio/files/asio/1.18.0%20%28Stable%29/asio-1.18.0.tar.gz" | tar xvz && cd asio-1.18.0/include && rm Makefile* && sudo cp -r * /usr/include/ && cd ../.. && rm -rf asio-1.18.0
```

You also need libcurl4 & google test with mocks on your system to run tests :
```bash
# libcurl4
sudo apt update -y && sudo apt-get install -y libcurl4 libcurl4-openssl-dev
# google test
git clone https://github.com/google/googletest.git && cd googletest && mkdir build && cd build && cmake .. && make && sudo make install && cd ../.. && rm -rf googletest
```

## Run tests & build

### On your computer

To build both libraries (static & dynamic) just use :
```bash 
    ./build.sh
```

You can also compile tests with :
```bash 
    ./build.sh dev
```
It is also possible to clean everything by using :
```bash 
    ./build.sh clean
```

### Using Docker
In order to test the project with docker you have to make sure your environment is up by using.
```bash 
    docker-compose up -d --build
```

Then you can start your tests with this command:
```bash 
    docker-compose exec tester ./build.sh dev
```

## Installing the libraries:

In order to install the library (both static & dynamic) you can do :

```bash
    mkdir -p build && cd build && cmake .. && sudo make install
```

## How to use?
For more information on how to use this library please go to this (example/README.md)

## Benchmarking

For more information about benchmarking please go to this [README](tests/benchmark/README.md)

## Disclaimer

This project is under development and is not currently usable in a production environment.\
Please feel free to ask features or/and send me any bug report.
