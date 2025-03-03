{
  description = "Description for the project";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux"];
      perSystem = { config, self', inputs', pkgs, system, ... }: {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            fmt_11
          ];

          # Custom prompt to show you're in the dev environment
          shellHook = ''
            # Use tput for colors instead of ANSI escape sequences
            yellow=$(tput setaf 1)
            green=$(tput setaf 2)
            blue=$(tput setaf 4)
            bold=$(tput bold)
            reset=$(tput sgr0)

            # Set custom prompt with tput colors
            export PS1="''${bold}''${green}[cpp-dev]''${reset} ''${bold}''${blue}\w''${reset} $ "

            # Create helpful aliases
            alias configure='cmake -B build'
            alias build='test -e build || echo ''${bold}''${yellow}configure first''${reset}; cmake --build build -j $(nproc || sysctl -n hw.ncpu || echo 4)'
            alias run='test -e ./build/main && ./build/main'

            echo "''${bold}''${green}=== C++ Development Environment ===''${reset}"
            echo "Available commands:"
            echo "  configure - Sets up the build system"
            echo "  build     - Compiles the project"
            echo "  run       - Runs the project"
            echo ""
          '';
        };
      };
    };
}
