shaderc -f vs_basic.sc -o build/Debug/shaders/dx9/vs_basic.bin --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef varying.def.sc -p s_3_0

shaderc -f fs_basic.sc -o build/Debug/shaders/dx9/fs_basic.bin --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef varying.def.sc -p s_3_0