
rm t.out
clear

cmake -S . -B build
rm ./build/syncronized
cmake --build ./build
time ./build/syncronized