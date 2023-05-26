#!/usr/bin/bash

gcc ./.build/build_custom.c -o ./.bin/build_custom -lraylib

gcc ./.build/build.c -o ./.bin/build
./.bin/build
