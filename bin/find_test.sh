#!/bin/sh
# find_test.sh : testing cat on each library

echo "\nFirst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-ff.so find . -name README.md
echo "\nNext Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-nf.so find . -name README.md
echo "\nBest Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-bf.so find . -name README.md
echo "\nWorst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-wf.so find . -name README.md
