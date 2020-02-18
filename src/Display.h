#ifndef DISPLAY_H
#define DISPLAY_H

#include <vita2d.h>

#include <string>

#include "Chip8.h"

#ifdef DEBUG_CHIP8
#include <chrono>
#endif

extern unsigned int defaultColour;
extern unsigned int selectedColour;
extern unsigned int titleColour;
extern unsigned int white;
extern unsigned int black;

class Display
{
    public:
        Display();
        ~Display();

        void beginFrame(bool clearScreen);
        void endFrame();

        void print(int x, int y, unsigned int colour, std::string string);
        void print(int x, int y, unsigned int colour, float scale, std::string string);
        void printCenter(int x, int y, unsigned int colour, std::string string);
        void printCenter(int x, int y, unsigned int colour, float scale, std::string string);

        void drawChip8(Chip8& chip8);

        int textHeight[4];

        vita2d_pgf *pgf;

    private:
        vita2d_texture* texture;
        unsigned int* texture_data;

        #ifdef DEBUG_CHIP8

            std::chrono::time_point<std::chrono::high_resolution_clock> startUpdate;
	        std::chrono::time_point<std::chrono::high_resolution_clock> stopUpdate;
	        int32_t  durationUpdate;
            std::chrono::time_point<std::chrono::high_resolution_clock> startDraw;
	        std::chrono::time_point<std::chrono::high_resolution_clock> stopDraw;
	        int32_t  durationDraw;

        #endif
};

#endif