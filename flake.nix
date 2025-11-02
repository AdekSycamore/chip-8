{
  description = "CHIP-8 Emulator";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "chip8-emulator";
          version = "1.0.0";
          
          src = builtins.path { path = ./.; name = "source"; };
          
          nativeBuildInputs = with pkgs; [
            cmake
            pkg-config
          ];
          
          buildInputs = with pkgs; [
            ncurses
          ];
          
          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];

          installPhase = ''
            runHook preInstall
            
            mkdir -p $out/bin
            cp chip8 $out/bin/chip8-emulator
            
            runHook postInstall
          '';
          
          meta = with pkgs.lib; {
            description = "CHIP-8 emulator with ncurses terminal graphics";
            homepage = "https://github.com/AdekSycamore/chip-8";
            license = licenses.mit;
            maintainers = [ ];
            platforms = platforms.unix;
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            pkg-config
            ncurses
          ];
          
          shellHook = ''
            echo "CHIP-8 Emulator Development Environment"
            echo "Available commands:"
            echo "  mkdir build && cd build && cmake .. && make  - Build the project"
            echo "  ./chip8 <rom>        - Run emulator"
          '';
        };
      });
}
