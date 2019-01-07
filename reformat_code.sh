#!/bin/bash

echo "Reformatting all *.cpp *.hpp files in $(pwd)"

clang-format -i -style="{IndentWidth: '4'}" src/*.cpp include/*.hpp

echo "Reformatting completed"
