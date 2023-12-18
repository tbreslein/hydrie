{
  description = "language/toolchain agnostic monorepo tool";
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        clang_stdenv = pkgs.llvmPackages_16.stdenv;
        gcc_stdenv = pkgs.gcc13Stdenv;

        # TODO: declare nanobench as build input
        buildInputs = with pkgs; [
          # build tools
          cmake
          ninja
          gcc13
          pkg-config

          # deps
          blaze
          pkgsStatic.fmt
          doctest
          cppcheck

          # dev tools
          neocmakelsp
          cmake-format
        ];

        mkExample = example:
          gcc_stdenv.mkDerivation {
            name = "${example}";
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
              cp build/examples/${example} $out/bin/
              cp build/compile_commands.json $out/
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
