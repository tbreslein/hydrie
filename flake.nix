{
  description = "language/toolchain agnostic monorepo tool";
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        # https://discourse.nixos.org/t/allow-insecure-packages-in-flake-nix/34655/2
        # pkgs = nixpkgs.legacyPackages.${system};
        pkgs = (import nixpkgs {
          inherit system;
          config.allowUnsupportedSystem = true;
        });

        clang_stdenv = pkgs.llvmPackages_16.stdenv;
        gcc_stdenv = pkgs.gcc13Stdenv;

        # TODO: declare the external libraries (blaze, fmt, doctest, nanobench)
        # as deps, instead of using CPM
        buildInputs = with pkgs; [
          # build tools
          cmake
          ninja
          gcc13
          pkg-config

          # deps
          blaze
          openblas
          pkgsStatic.fmt
          doctest
          cppcheck
          # still need add either chronoxor/cppbenchmark or martinus/nanobench

          # dev tools
          neocmakelsp
          cmake-format
        ];

        mkExample = example:
          gcc_stdenv.mkDerivation {
            name = "${example} example";
            inherit buildInputs;
            src = ./.;
            buildPhase = ''
              cmake -S$src -Bbuild -G"Ninja" \
                -DCMAKE_BUILD_TYPE="Release" \
                -DEXAMPLES="${example}" \
                -DHYDRIE_TEST="OFF" \
                -DHYDRIE_BENCH="OFF"
              cmake --build build
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp -r build/examples/${example} $out/bin/
            '';
            CC = "${pkgs.gcc13}/bin/gcc";
            CXX = "${pkgs.gcc13}/bin/g++";
          };
      in {
        # https://discourse.nixos.org/t/how-to-select-a-specific-version-of-gcc-within-a-flake-nix/23372/6
        devShells.default = (pkgs.mkShell.override { stdenv = clang_stdenv; }) {
          # (pkgs.mkShell.override { stdenv = gcc_stdenv; }) {
          buildInputs = buildInputs ++ (with pkgs; [
            clang_16
            llvmPackages_16.openmp
            cppcheck
            go-task
          ]);
          CC = "${pkgs.clang_16}/bin/clang";
          CXX = "${pkgs.clang_16}/bin/clang++";
        };

        packages.minimal = mkExample "minimal";
      });
}
