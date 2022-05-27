#!/bin/bash

clang++ -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined -g3 -O0 main.cpp --std=c++98 -ferror-limit=$1 && ./a.out