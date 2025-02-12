#!/bin/sh

DIR="folly"

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    ./a.out $f
done
