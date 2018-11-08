
#!/bin/bash

## Compile our two program files
g++ -std=c++14 -Ofast -march=native -o code main.cpp Agent.cpp Board.cpp
