#!/bin/sh
# du_test.sh : testing cat on each library

echo "\nFirst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-ff.so du
echo "\nNext Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-nf.so du
echo "\nBest Fit  ==================================================================="
time env LD_PRELOAD=lib/libmalloc-bf.so du
echo "\nWorst Fit ==================================================================="
time env LD_PRELOAD=lib/libmalloc-wf.so du
