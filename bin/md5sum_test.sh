#!/bin/sh
# md5sum_test.sh : testing cat on each library

echo "\nFirst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-ff.so md5sum README.md
echo "\nNext Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-nf.so md5sum README.md
echo "\nBest Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-bf.so md5sum README.md
echo "\nWorst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-wf.so md5sum README.md
