#!/bin/sh

if make lib/libmalloc-ff.so; then
    env LD_PRELOAD=lib/libmalloc-ff.so cat README.md
else
    echo "Error in make"
fi
