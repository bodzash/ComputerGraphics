import os
import sys
import subprocess

# TODO: implement unix timestamping of shit loool :D
# TODO: from bgfx_shader.sh remove the predefined uniforms at the bottom

"""""
    Argument list
    -dxlevel | 9, 11, 12  | DirectX version.                   | Defaults to 9
    -olevel  | 0, 1, 2, 3 | Optimization level. (DX9 and DX11) | Defaults to 0
"""

# Arguments
argc = len(sys.argv)
argv = sys.argv[1:]

# Path definitions
in_folder: str = "./Shaders/"
out_folder: str = "./D3D9/" # TODO: remove hard coded
compiler_path: str = "./Binaries/Win32/shaderc"
varying_path: str = "./Shaders/Varying.def"
include_path: str = "./Shaders/Include/"

# Apply arguments
#opt_dxlevel = argv.index("-dxlevel")
#out_folder = "./D3D" + argv[argv.index("-dxlevel") + 1]

# Functions
def compile_vertex(shader: str):
    in_path: str = f"-f {in_folder}{shader}"
    out_path: str = f"-o {out_folder}{shader}".replace(".vert", ".bvs")
    type: str = "--type vertex"
    include: str = f"-i {include_path}"
    platform: str = "--platform windows"
    varying: str = f"--varyingdef {varying_path}"
    profile: str = "-p s_3_0" # TODO: remove hard coded profile version
    optlevel: str = "-O 3" # TODO: remove hard coded optimization level

    command = [compiler_path, in_path, out_path, type, include, platform, varying, profile, optlevel]
    exitcode = subprocess.run(" ".join(command).split(" ")).returncode

    exitcode or print(f"Compiled {shader}")
    exitcode and print(f"Failed {shader}")

def compile_fragment(shader: str):   
    in_path: str = f"-f {in_folder}{shader}"
    out_path: str = f"-o {out_folder}{shader}".replace(".frag", ".fvs")
    type: str = "--type fragment"
    include: str = f"-i {include_path}"
    platform: str = "--platform windows"
    varying: str = f"--varyingdef {varying_path}"
    profile: str = "-p s_3_0" # TODO: remove hard coded profile version
    optlevel: str = "-O 3" # TODO: remove hard coded optimization level

    command = [compiler_path, in_path, out_path, type, include, platform, varying, profile, optlevel]
    exitcode = subprocess.run(" ".join(command).split(" ")).returncode

    exitcode or print(f"Compiled {shader}")
    exitcode and print(f"Failed {shader}")

# Main
print("\n--- Shader Compiler Script ---\n")

for f in os.listdir(in_folder):
    if f.endswith(".frag"):
        compile_fragment(f)
        continue

    if f.endswith(".vert"):
        compile_vertex(f)