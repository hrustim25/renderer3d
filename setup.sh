#!/bin/sh

sudo apt install libsfml-dev
sudo apt install libjpeg-dev
sudo apt install libpng-dev

mkdir build
cd build
cmake ..
make