shaderc -f Basic.vert -o build/Debug/shaders/dx9/Basic.vert.bin --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef Basic.def -p s_3_0
shaderc -f Basic.frag -o build/Debug/shaders/dx9/Basic.frag.bin --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef Basic.def -p s_3_0



shaderc -f Light.vert -o build/Debug/shaders/dx9/vs_light.bin --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef Light.def -p s_3_0
shaderc -f Light.frag -o build/Debug/shaders/dx9/fs_light.bin --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef Light.def -p s_3_0