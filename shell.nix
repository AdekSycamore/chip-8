{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    pkg-config
    ncurses
    gdb
  ];
  
  shellHook = ''
    echo "CHIP-8 Emulator Development Environment"
    echo "Run 'mkdir -p build && cd build && cmake .. && make' to build"
  '';
}
