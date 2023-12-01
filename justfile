# build program
build: 
  cmake --build build

# generate build
gen-build:
  mkdir -p build
  cd build && cmake ..
  cd ..

# run program
run: build
  ./build/app

# remove build files
clean:
  cmake --build build --target clean
