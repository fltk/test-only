#!/bin/sh

autoconf

CC=cc
CXX=CC
export CC CXX

./configure $*

make clean depend all
