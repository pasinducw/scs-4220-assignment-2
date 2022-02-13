#!/bin/bash

./cmake-build-debug/assignment2 $1 $2 $3 1 > results-$1-$2-$3.csv
for i in {1..1000}
  do
    ./cmake-build-debug/assignment2 $1 $2 $3 0 >> results-$1-$2-$3.csv
done
