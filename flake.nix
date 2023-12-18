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
        ];

        mkPkg = { name, buildType ? "Release", tests ? false, bench ? false }:
          gcc_stdenv.mkDerivation {
            inherit buildInputs name;
            src = ./.;
            buildPhase = ''
              cmake -S$src -Bbuild -G"Ninja" \
                -DCMAKE_BUILD_TYPE="${buildType}" \
                -DEXAMPLES="${name}" \
                -DHYDRIE_TEST="${if tests then "ON" else "OFF"}" \
                -DHYDRIE_BENCH="${if bench then "ON" else "OFF"}"
              cmake --build build
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp -fr build/examples/${name} $out/bin/
              cp build/compile_commands.json $out/
            '';
            CC = "${pkgs.gcc13}/bin/gcc";
            CXX = "${pkgs.gcc13}/bin/g++";
          };

        mkExample = name:
          mkPkg {
            inherit name;
            buildType = "Release";
            tests = true;
            bench = true;
          };

        availableExamples = [ "minimal" ];
        # availableExamples =
        #   let examples = pkgs.lib.sourceFilesBySuffices ./examples [ ".cpp" ];
        #   in map (x:
        #     builtins.head
        #     (builtins.split ".cpp$" (builtins.baseNameOf toString x))) examples;
      in {
        # https://discourse.nixos.org/t/how-to-select-a-specific-version-of-gcc-within-a-flake-nix/23372/6
        devShells.default = (pkgs.mkShell.override { stdenv = clang_stdenv; }) {
          # (pkgs.mkShell.override { stdenv = gcc_stdenv; }) {
          buildInputs = buildInputs ++ (with pkgs; [
            clang_16
            llvmPackages_16.openmp
            cppcheck
            neocmakelsp
            cmake-format
          ]);
          CC = "${pkgs.clang_16}/bin/clang";
          CXX = "${pkgs.clang_16}/bin/clang++";
        };

        packages = pkgs.lib.genAttrs availableExamples (x: mkExample x);
      });
}
