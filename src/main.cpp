#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h> 
#include <vita2d.h>

#include <debugnet.h>

#include "Display.h"
#include "GameSelect.h"
#include "Chip8.h"

#define ip_server "192.168.1.228"
#define port_server 18194

const int delayInterval = 150000;

int main()
{
	#ifdef DEBUG_CHIP8
	int ret;
	ret=debugNetInit(ip_server,port_server,DEBUG);
	#endif

	Display display = Display();

	Chip8 chip8 = Chip8();

	// Setup input
	SceCtrlData ctrl;
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITAL); // Disable analogue

	// Variables for menus
	enum Screen { menu, gameSelectScreen, emulatorScreen, about };
	Screen screen = menu;
	int selectedLabel = 0;
	int maxLabels = 0;

	// Other screens
	GameSelect gameSelect = GameSelect();

	bool doRun = true;
	while(doRun)
	{
		sceCtrlPeekBufferPositive(0, &ctrl, 1);

		display.beginFrame();

		if (screen != emulatorScreen && screen != about)
		{
			if (ctrl.buttons & SCE_CTRL_DOWN) selectedLabel++;
			if (ctrl.buttons & SCE_CTRL_UP) selectedLabel--;

			if (selectedLabel > maxLabels) selectedLabel = maxLabels;
			if (selectedLabel < 0) selectedLabel = 0;
		}

		switch(screen)
		{
			case menu:

				display.printCenter(960/2, 544/3, titleColour, 2.0f, "Chip8 Vita");

				display.printCenter(960/2, 300 + display.textHeight[1] * 0, (selectedLabel == 0) ? selectedColour : defaultColour, 1.0f, "Select game");
				display.printCenter(960/2, 300 + display.textHeight[1] * 1, (selectedLabel == 1) ? selectedColour : defaultColour, 1.0f, "About");
				display.printCenter(960/2, 300 + display.textHeight[1] * 2, (selectedLabel == 2) ? selectedColour : defaultColour, 1.0f, "Quit");

				maxLabels = 2;
				if (ctrl.buttons & SCE_CTRL_CROSS)
				{
					if (selectedLabel == 0) screen = gameSelectScreen;
					if (selectedLabel == 1) screen = about;
					if (selectedLabel == 2) doRun = false;
				}

			break;

			case gameSelectScreen:
				screen = (Screen) gameSelect.renderFrame(selectedLabel, maxLabels, display, ctrl, chip8);
			break;

			case emulatorScreen:

				chip8.doEmulationCycle();
				display.drawChip8(chip8);

				if (ctrl.buttons & SCE_CTRL_START) screen = gameSelectScreen;

			break;

			case about:

				display.printCenter(960/2, 544/3, titleColour, 2.0f, "Chip8 Vita");

				display.printCenter(960/2, 300 + display.textHeight[1] * 0, defaultColour, 1.0f, "This Chip8 Emulator uses Vita2D, as well as Debugnet.");
				display.printCenter(960/2, 300 + display.textHeight[1] * 1, defaultColour, 1.0f, "Made by Luka Warren (TheUltimateKerbonaut / Kerbo)");
				display.printCenter(960/2, 300 + display.textHeight[1] * 2, defaultColour, 1.0f, "https://github.com/TheUltimateKerbonaut");

				maxLabels = 0;
				if (ctrl.buttons & SCE_CTRL_CIRCLE) screen = menu;

			break;

			default:

			break;
		}
		if (ctrl.buttons && screen != emulatorScreen) sceKernelDelayThread(delayInterval); // Delay for input
		else display.printCenter(960/2, 544/3, titleColour, 2.0f, "Chip8 Vita");

		display.endFrame();
	}
	
	#ifdef DEBUG_CHIP8
	debugNetFinish();
	#endif

	sceKernelExitProcess(0);
	return 0;
}