#!/bin/sh
# cat_test.sh : testing cat on each library

echo "\nFirst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-ff.so cat README.md
echo "\nNext Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-nf.so cat README.md
echo "\nBest Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-bf.so cat README.md
echo "\nWorst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-wf.so cat README.md
