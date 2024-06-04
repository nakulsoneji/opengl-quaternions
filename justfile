# build program
build:
  @echo "building app..."
  cmake --build build

# generate build
gen-build:
  @echo "generating build..."
  cmake -S . -B build

# run program
run: build
  @echo "running app..."
  ./build/app

# remove build files (basically just the binary)
clean:
  @echo "cleaning..."
  cmake --build build --target clean

# cleans and builds all in one (clean + build)!!
fresh: 
  @echo "cleaning and building..."
  cmake --build build --clean-first
