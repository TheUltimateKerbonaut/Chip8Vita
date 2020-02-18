#include "ROM.h"

#include <psp2/io/fcntl.h> 

#include <debugnet.h>

#include <string>
#include <cstring>

ROM::ROM(std::string fileName, SceOff size)
{
	std::string filePath = "app0:/roms/" + std::string(fileName);

	SceUID fd = sceIoOpen(filePath.c_str(), SCE_O_RDONLY, 0777);
	if (fd > 0)
	{
		buffer = std::vector<unsigned char>(size);
		sceIoRead(fd, buffer.data(), size);
	}
	else
    {
        #ifdef DEBUG_CHIP8
		    debugNetPrintf(DEBUG,"Error with loading ROM %s: 0x%X\n", filePath.c_str(), (unsigned int)fd);
	    #endif
    }

	sceIoClose(fd);

	#ifdef DEBUG_CHIP8
		debugNetPrintf(DEBUG,"Loaded ROM %s\n", filePath.c_str());
	#endif

}