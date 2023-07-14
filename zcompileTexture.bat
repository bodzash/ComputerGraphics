@ECHO OFF
ECHO [INFO]: Compiling textures...

REM Regular model textures shold use ETC1 or the other one
REM texturec -f ./Assets/Jack/HandsomeJack_Diff.png -o ./build/Debug/Jack/HandsomeJack_Diff.dds --mips -t ETC1
REM texturec -f ./Assets/Jack/HandsomeJackMask_Diff.png -o ./build/Debug/Jack/HandsomeJackMask_Diff.dds --mips -t ETC1

REM Transparent textures should use RGBA8 or BGRA8
REM texturec -f ./Assets/Grass.png -o ./build/Debug/Grass.dds --mips -t RGBA8

REM Skybox textures should use NO mipmaps and RGB8 or another losless format
REM texturec -f ./Assets/Skybox.png -o ./build/Debug/Skybox.dds --strip -t RGB8
texturec -f ./Assets/SkyboxDay.png -o ./build/Debug/SkyboxDay.dds --strip -t RGB8

@ECHO ON

@ECHO OFF
PAUSE