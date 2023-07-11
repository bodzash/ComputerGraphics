@ECHO OFF
ECHO [IMPORTANT]: Texture needs to be flipped vertically before loading!
ECHO [INFO]: Compiling textures...

REM texturec -f ./Assets/Jack/HandsomeJack_Diff.png -o ./build/Debug/Jack/HandsomeJack_Diff.dds --mips -t ETC1
REM texturec -f ./Assets/Jack/HandsomeJackMask_Diff.png -o ./build/Debug/Jack/HandsomeJackMask_Diff.dds --mips -t ETC1

texturec -f ./Assets/Grass.png -o ./build/Debug/Grass.dds --mips -t RGBA8

@ECHO ON

@ECHO OFF
PAUSE