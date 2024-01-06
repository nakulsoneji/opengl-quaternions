# build program
build:
  cmake --build build

# generate build
gen-build:
  cmake -S . -B build

# run program
run: build
  ./build/app

# remove build files (basically just the binary)
clean:
  cmake --build build --target clean

# cleans and builds all in one (clean + build)!!
fresh: 
  cmake --build build --clean-first
