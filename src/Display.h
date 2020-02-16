#ifndef DISPLAY_H
#define DISPLAY_H

#include <vita2d.h>

#include <string>

#include "Chip8.h"

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

        void beginFrame();
        void endFrame();

        void print(int x, int y, unsigned int colour, std::string string);
        void print(int x, int y, unsigned int colour, float scale, std::string string);
        void printCenter(int x, int y, unsigned int colour, std::string string);
        void printCenter(int x, int y, unsigned int colour, float scale, std::string string);

        void drawChip8(Chip8& chip8);

        int textHeight[4];

    private:
        vita2d_pgf *pgf;
        vita2d_texture* texture;
        unsigned int* texture_data;
};

#endif