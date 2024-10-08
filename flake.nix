{
  description = "A BASIC compiler for RISC-V written in C";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
		flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system: let
		pkgs = import nixpkgs { inherit system; };

		pkg-name = "basic";
		build-deps = [ pkgs.clang ];
	in {
		defaultPackage = pkgs.stdenv.mkDerivation {
			name = pkg-name;
			src = ./.;

			nativeBuildInputs = build-deps;

			buildPhase = ''
				make build --always-make CC_ARGS=-O3 BUILD_DIR=./
			'';

			installPhase = ''
				mkdir -p $out/bin
				cp ${pkg-name} $out/bin
			'';
		};

		devShells = {
			default = pkgs.mkShell {
				buildInputs = build-deps ++ (with pkgs; [
						valgrind
				]);
			};

			emulator = let
			  riscv-pkgs = import <nixpkgs> {
					crossSystem = (import <nixpkgs/lib>).systems.examples.riscv32;
				};
			in riscv-pkgs.mkShell {
				depsBuildBuild = [
					riscv-pkgs.coreutils
					pkgs.qemu
				];
			};
		};
	});
}
