#include "ROM.h"

#include <psp2/io/fcntl.h> 

#include <debugnet.h>

#include <string>

ROM::ROM(std::string fileName, SceOff size)
{
	std::string filePath = "app0:/roms/" + std::string(fileName);

	SceUID fd = sceIoOpen(filePath.c_str(), SCE_O_RDONLY, 0777);
	if (fd > 0)
	{
		void* voidBuf;
		sceIoRead(fd, voidBuf, size);
		unsigned char *charBuf = (unsigned char*)voidBuf;
		buffer = std::vector<unsigned char>(charBuf, charBuf + size);

		#ifdef DEBUG_CHIP8
		    for (int i =0; i < buffer.size(); ++i)
				debugNetPrintf(DEBUG,"0x%X\n", buffer[i]);
	    #endif
	}
	else
    {
        #ifdef DEBUG_CHIP8
		    debugNetPrintf(DEBUG,"Error with loading ROM %s: 0x%X\n", filePath.c_str(), (unsigned int)fd);
	    #endif
    }

	sceIoClose(fd);
}