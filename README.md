# Cpp Http
Make websites with cpp.

## Before You Begin
You need to install cmake & google test on your system or use docker.

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

* bin: test & app binaries.

## How does it work?

To make it work just use "./build.sh". 

You can also compile tests in debug mode with "./build.sh dev".

It is also possible to clean everything by using "./build.sh clean".
