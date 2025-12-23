{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.clang_20
    pkgs.gcc14
  ];
  
  shellHook = ''
    export CC=clang
    export CXX=clang++
  '';
}
