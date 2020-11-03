# GoogleTest
Blank project using google unit testing. It works on Linux.

## Before You Begin
You need to install cmake, google & fseam on your system or use docker.

## Docker
In order to test your app with docker you have to make sure your environment is up by using.
```bash 
    docker-compose up -d --build
```

Then you can start your tests with this command:
```bash 
    docker-compose exec tester ./build.sh dev
```


## Folder organisation
* src: sources of your project.

* tests: google unit tests.

* build: cmake genarated files.

* bin: test & app binaries.

## How does it work?

To make it work just use "./build.sh". 

You can also compile tests in debug mode with "./build.sh dev".

Your application and test binaries will be placed in the bin folder.

It is also possible to clean everything by using "./build.sh clean".

You can modify this script if you need to change/update your folder organsation, your application name, etc...

