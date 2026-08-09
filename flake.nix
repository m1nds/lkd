{
  description = "LKD Dev Flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        llvmSet = pkgs.llvmPackages_18;
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            llvmSet.clang
            llvmSet.llvm
            llvmSet.lld
            llvmSet.clang-tools

            limine
            xorriso
            mtools
            gptfdisk

            cmake
            ninja
            gnumake
            nasm
            automake
            autoconf
            bison
            flex

            qemu
            OVMF
            gdb
          ];

          shellHook = ''
            export OVMF_DIR="${pkgs.OVMF.fd}/FV"
          '';
        };
      });
}
