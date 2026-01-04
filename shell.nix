{ pkgs ? import <nixpkgs> { } }:
let
    inherit (pkgs) mkShell;
    inherit (pkgs.lib) makeLibraryPath;

    dependencyLibraries = with pkgs; [
        # Libraryes to use
    ];
in
    mkShell {
        packages = (with pkgs; [
            gcc
            gnumake
            gdb
        ]) ++ dependencyLibraries;

        shellHook = ''
        export CC=gcc
        export CXX=g++
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ makeLibraryPath dependencyLibraries }
        '';
    }
