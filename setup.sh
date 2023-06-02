#!/bin/sh

sudo apt install libsfml-dev

mkdir build
cd build
cmake ..
make
