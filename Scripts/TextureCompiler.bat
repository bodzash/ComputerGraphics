REM Regular model textures shold use ETC1
REM Transparent textures should use RGBA8 or BGRA8
REM Skybox textures should use NO mipmaps and RGB8 or another losless format
START ../Binaries/Win32/texturec -f ../Content/Models/TestSkins/Ashe/ashe_woadleader_2012_TX_CM.png -o ../Content/Models/TestSkins/Ashe/ashe_woadleader_2012_TX_CM.dds --mips -t ETC1