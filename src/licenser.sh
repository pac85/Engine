#!/bin/bash

shopt -s globstar

for i in *.c *.cpp *.h *.hpp ./**/*.c ./**/*.cpp ./**/*.h ./**/*.hpp # or whatever other pattern...
do
  if ! grep -q Copyright $i
  then
    cat toadd.txt $i >$i.new && mv $i.new $i
  fi
done
