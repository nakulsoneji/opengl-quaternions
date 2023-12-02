# build program
build: 
  meson compile -C build

# generate build
gen-build:
  meson setup build
  ln -s build/compile_commands.json compile_commands.json

# run program
run: build
  ./build/app

# remove build files
clean:
  meson compile -C build --clean
