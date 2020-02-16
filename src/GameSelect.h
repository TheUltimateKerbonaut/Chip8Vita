#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#include <psp2/io/dirent.h> 
#include <psp2/ctrl.h> 

#include <cstring> 

#include "Display.h"
#include "Chip8.h"

struct RomFile
{
    char name[256];
    SceIoStat metadata;
};

class GameSelect
{
    public:
        GameSelect();
        ~GameSelect();

        int renderFrame(int selectedLabel, int& maxLabels, Display& display, SceCtrlData& ctrl, Chip8& chip8);
    
    private:
        RomFile romFiles[256];
        unsigned int fileCount = 0;
        int scrollAmount = 0;
};

#endif