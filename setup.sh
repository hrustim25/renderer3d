#!/bin/sh

sudo apt install libsfml-dev
sudo apt install libjpeg-dev

mkdir build
cd build
cmake ..
make