# GAS
Game Asset Storage

This is a university project.

It allows to store and manage different assets used in game development process in database.

Currently db of choice is Postgresql.

## Disclamer

This is not finished by any means project. It is full of bugs and requires a lot of work to be completed.

## Instructions for build:
All needed dependencies can be obtained with
[conan](https://docs.conan.io/en/latest/installation.html)

Also gui part is using Qt 5.14

Steps:
~~~~
$ git clone https://github.com/ShadowCurse/GAS.git
$ cd GAS
$ mkdir build && cd build
$ conan install .. --build=missing
$ make -j
~~~~

Also it worth mentioning that currently project is build with clang, so `conan` config should contain 
~~~~
...
compiler=clang
...
compiler.libcxx=libstdc++11
...
[env]
CC=/usr/bin/clang
CXX=/usr/bin/clang++
~~~~
it can be set up using 
~~~~
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler=clang default
$ conan profile update settings.compiler.libcxx=libstdc++11 default
$ conan profile update env.CC=/usr/bin/clang default
$ conan profile update env.CXX=/usr/bin/clang++ default
~~~~

