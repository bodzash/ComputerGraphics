@ECHO OFF
ECHO [IMPORTANT]: Texture needs to be flipped vertically before loading!
ECHO [INFO]: Compiling textures...

REM texturec -f ./Assets/Angel_Diff.png -o ./build/Debug/Angel_Diff.dds --mips -t RGB8
REM texturec -f ./Assets/Angel_Spec.png -o ./build/Debug/Angel_Spec.dds --mips -t RGB8

REM texturec -f ./Assets/DuelIndicator_Diff.png -o ./build/Debug/DuelIndicator_Diff.dds --mips -t RGB8
REM texturec -f ./Assets/DuelIndicator_Spec.png -o ./build/Debug/DuelIndicator_Spec.dds --mips -t RGB8

texturec -f ./Assets/Jack/HandsomeJack_Diff.png -o ./build/Debug/Jack/HandsomeJack_Diff.dds --mips -t ETC1
texturec -f ./Assets/Jack/HandsomeJackMask_Diff.png -o ./build/Debug/Jack/HandsomeJackMask_Diff.dds --mips -t ETC1

@ECHO ON

@ECHO OFF
PAUSE