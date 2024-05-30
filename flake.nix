{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        cmake
        glfw
        just
        glm
        ffmpeg
        imgui
        libGL
      ];
      shellHook = ''
        export LD_LIBRARY_PATH="/run/opengl-driver-32/lib:/run/opengl-driver/lib/:${pkgs.wayland}/lib:$LD_LIBRARY_PATH"
        export IMGUI_DIR="${pkgs.imgui}/include/imgui"
        exec zsh
      '';
    };
  };
}
