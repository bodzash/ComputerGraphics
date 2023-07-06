@ECHO OFF
ECHO [IMPORTANT]: Texture needs to be flipped vertically before loading!
ECHO [INFO]: Compiling textures...

texturec -f ./Assets/viking_room.png -o ./build/Debug/viking_room.dds --mips -t RGB8
@ECHO ON