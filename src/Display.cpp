#include "Display.h"

#include <psp2/display.h>

unsigned int defaultColour = RGBA8(255, 0, 0, 255);
unsigned int selectedColour = RGBA8(0, 255, 0, 255);
unsigned int titleColour = RGBA8(0, 0, 255, 255);
unsigned int white = RGBA8(255, 255, 255, 255);
unsigned int black = RGBA8(0, 0, 0, 255);

Display::Display()
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));
	pgf = vita2d_load_default_pgf();
	//vita2d_set_vblank_wait(1);

	for (int i = 0; i < sizeof(textHeight) / sizeof(textHeight[0]); ++i)
		textHeight[i] = vita2d_pgf_text_width(pgf, i+1, "A");

	texture = vita2d_create_empty_texture(64, 32);
	texture_data = (unsigned int*) vita2d_texture_get_datap(texture);
}

void Display::beginFrame()
{
    vita2d_start_drawing();
	vita2d_clear_screen();
}

void Display::endFrame()
{
    vita2d_end_drawing();
	vita2d_swap_buffers();
	vita2d_wait_rendering_done();
	//sceDisplayWaitVblankStart();
}

void Display::print(int x, int y, unsigned int colour, std::string string)
{
    vita2d_pgf_draw_text(pgf, x, y, colour, 1.0f, string.c_str());
}

void Display::print(int x, int y, unsigned int  colour, float scale, std::string string)
{
    vita2d_pgf_draw_text(pgf, x, y, colour, scale, string.c_str());
}

void Display::printCenter(int x, int y, unsigned int colour, std::string string)
{
	vita2d_pgf_draw_text(pgf, x - vita2d_pgf_text_width(pgf, 1.0f, string.c_str())/2, y - vita2d_pgf_text_height(pgf, 1.0f, string.c_str())/2, colour, 1.0f, string.c_str());
}

void Display::printCenter(int x, int y, unsigned int colour, float scale, std::string string)
{
	vita2d_pgf_draw_text(pgf, x - vita2d_pgf_text_width(pgf, scale, string.c_str())/2, y - vita2d_pgf_text_height(pgf, scale, string.c_str())/2, colour, scale, string.c_str());
}

void Display::drawChip8(Chip8& chip8)
{
	// Draw display
	if (chip8.cpu.drawFlag)
	{
		for (unsigned int x = 0; x < 64; ++x)
		{
			for (unsigned int y = 0; y < 32; ++y)
			{
				texture_data[x+y*64] = chip8.io.pixels[x+y*64] ? white : black;
			}
		}
		chip8.cpu.drawFlag = false;
	}

	vita2d_draw_texture_scale(texture, 0, 32, 15, 15);

	printCenter(960/2, 30, RGBA8(255, 255, 255, 255), 1.0f, "Press start to go back");
}

Display::~Display()
{
	vita2d_free_texture(texture);
    vita2d_fini();
	vita2d_free_pgf(pgf);
}