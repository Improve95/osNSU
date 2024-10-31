rm t.out
rm ./build/syncronized
clear

# cmake -S . -B build

cmake --build ./build
time ./build/syncronized