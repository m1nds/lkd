with import <nixpkgs> {};

clangStdenv.mkDerivation {
  name = "clang-nix-shell";
  buildInputs = [
    pkgs.clang_18
    pkgs.llvm_18
    pkgs.lld_18
    pkgs.ninja
    pkgs.cmake
    pkgs.qemu
    pkgs.glibc_multi
  ];
}
