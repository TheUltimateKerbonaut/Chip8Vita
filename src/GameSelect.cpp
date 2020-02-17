#include "GameSelect.h"

#include <psp2/io/dirent.h> 
#include <debugnet.h>

#include <string>

#include "ROM.h"

GameSelect::GameSelect()
{
    SceIoDirent dir;
    memset(&dir, 0, sizeof(SceIoDirent));


    SceUID dfd;
    dfd = sceIoDopen("app0:/roms");
    if(dfd >= 0)
    {
        while (sceIoDread(dfd, &dir) > 0)
        {
            if (strstr(dir.d_name, ".ch8") != NULL)
            {
                romFiles[fileCount] = RomFile();
                strncpy(romFiles[fileCount].name, dir.d_name, 256);
                romFiles[fileCount].metadata=dir.d_stat;
                fileCount++;
            }
        }
    }
    else
    {
        #ifdef DEBUG_CHIP8
		    debugNetPrintf(DEBUG,"Error with filesystem: 0x%X\n", (unsigned int)dfd);
	    #endif
    }
    
    sceIoDclose(dfd);

    #ifdef DEBUG_CHIP8
		debugNetPrintf(DEBUG,"Files: %d\n", fileCount);
        for (int i = 0; i < fileCount; ++i)
            debugNetPrintf(DEBUG,"%s\n", romFiles[i].name);
	#endif
}

int GameSelect::renderFrame(int selectedLabel, int& maxLabels, Display& display, SceCtrlData& ctrl, Chip8& chip8)
{

    display.printCenter(960/2, 544/3, titleColour, 2.0f, "Chip8 Vita");

    if (selectedLabel > 3) scrollAmount = selectedLabel - 3;
    else scrollAmount = 0;

    for (int i = scrollAmount; i < fileCount; ++i)
        display.printCenter(960/2, 300 + display.textHeight[1] * (i-scrollAmount), (selectedLabel == i) ? selectedColour : defaultColour, 1.0f, romFiles[i].name);

    if (fileCount == 0) display.printCenter(900/2, 300, defaultColour, "Error: No .ch8 files in ux0:app/LUKA00001/roms/");

	maxLabels = fileCount-1;
	if (ctrl.buttons & SCE_CTRL_CROSS && fileCount > 0)
	{
        chip8.init();
        chip8.loadROM(ROM(std::string(romFiles[selectedLabel].name), romFiles[selectedLabel].metadata.st_size));
		return 2;
	}
    if (ctrl.buttons & SCE_CTRL_CIRCLE)
    {
        return 0;
    }
    return 1;

}

GameSelect::~GameSelect()
{

}