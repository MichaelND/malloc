#!/bin/sh
# sort_test.sh : testing cat on each library

echo "\nFirst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-ff.so sort README.md
echo "\nNext Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-nf.so sort README.md
echo "\nBest Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-bf.so sort README.md
echo "\nWorst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-wf.so sort README.md
