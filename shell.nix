{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell rec {
    nativeBuildInputs = with pkgs; [
      gcc
      gdb
      gf
      cgdb
      clang

      llvmPackages_latest.bintools
      pkg-config
      gnumake
      bear
    ];

    buildInputs = with pkgs; [ ];

    LD_LIBRARY_PATH = "$LD_LIBRARY_PATH:${
      with pkgs; lib.makeLibraryPath buildInputs
    }";

    LIBCLANG_PATH = pkgs.lib.makeLibraryPath [
      pkgs.llvmPackages_latest.libclang.lib
    ];
  }
