alias c := config
alias b := build
alias e := example
alias t := test

src_dir := "."
build_dir := "build"
build_clang_dir := "build_clang"
bin_dir := "bin"
generator := "Ninja"
export CC := "gcc"
export CXX := "g++"
export CMAKE_EXPORT_COMPILE_COMMANDS := "YES"

config: clean
    CC="clang" CXX="clang++" cmake -S {{ src_dir }} -B {{ build_clang_dir }} -G {{ generator }}
    cmake -S {{ src_dir }} -B {{ build_dir }} -G {{ generator }}
    ln -sf {{ build_clang_dir }}/compile_commands.json ./compile_commands.json

build:
    if [ ! -d {{ build_dir }} ] || [ ! -d {{ build_clang_dir }} ]; then just config; fi
    cmake --build {{ build_dir }}

example name: build
    ./{{ build_dir }}/{{ name }}

test: build
    ctest --build {{ build_dir }}

clean:
    rm -fr {{ build_dir }}
    rm -fr {{ build_clang_dir }}
    rm -fr {{ bin_dir }}
