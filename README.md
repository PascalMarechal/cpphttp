# Cpp Http
Make websites with cpp.

## Before You Begin
You need to install cmake to build the library.\
You also need libcurl4 & gtest with mocks on your system to run tests.\
If you have docker you can also run your test using the docker-compose file. (check "Dockerfile.dev" for more information)

## Docker
In order to test the project with docker you have to make sure your environment is up by using.
```bash 
    docker-compose up -d --build
```

Then you can start your tests with this command:
```bash 
    docker-compose exec tester ./build.sh dev
```


## Folder organisation
* src: sources.

* tests: google unit tests.

* build: cmake genarated files.

* bin: test & library binaries.

## How does it work?

To make it work just use "./build.sh". 

You can also compile tests with "./build.sh dev".

It is also possible to clean everything by using "./build.sh clean".
