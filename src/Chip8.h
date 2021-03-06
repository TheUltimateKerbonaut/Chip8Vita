#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#include "ROM.h"

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>

/*

0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM

*/

struct CPU
{
	uint16_t opcode;
	uint8_t memory[4096]; // 4K memory
	uint8_t V[16]; // 15 8-bit registers, and a carry flag

	uint16_t indexRegister;
	uint16_t programCounter;

	uint16_t stack[16];
	uint16_t stackPointer;

	bool drawFlag;

};

struct IO
{
	uint8_t pixels[64 * 32]; // 64 x 32 screen of 1 bit pixels
	uint8_t keys[16]; // 0x0 - 0xF HEX based keypad
};

struct Timer
{
	uint8_t delayTimer;
	uint8_t soundTimer;
};

class Chip8
{
public:

	void init();
	void loadROM(ROM rom);
	void doEmulationCycle();

	void spawnEmulationThread(Chip8* chip8);
	void stopEmulationThread();

	IO io;
	CPU cpu;
	Timer timer;

	double FPS = 0;

private:

	const int seed = 1234;

	uint8_t fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	typedef struct {
		Chip8* chip8;
	} ThreadArguments;


	SceUID emulationThread;
	static bool continueEmulation;

	static int emulateChip8(SceSize args, ThreadArguments* argp);

};

#endif