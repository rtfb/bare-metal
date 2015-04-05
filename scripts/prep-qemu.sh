#!/bin/sh

rm -rf qemu
mkdir qemu
cd qemu
git clone https://github.com/Torlus/qemu src
cd src
git checkout rpi
mkdir ../build
cd ../build
../src/configure --prefix=. --target-list=arm-softmmu,arm-linux-user,armeb-linux-user --enable-sdl
make && make install
