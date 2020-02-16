#include "Chip8.h"

#include <iostream>
#include <random>

void Chip8::init() // Initialise registers and memory
{
	cpu.programCounter = 0x200; // Program counters starts at 0x200
	
	// Clear CPU state
	cpu.opcode = 0;
	cpu.indexRegister = 0;
	cpu.stackPointer = 0;

	for (unsigned int i = 0; i < sizeof(io.pixels) / sizeof(io.pixels[0]); ++i) io.pixels[i] = 0; // Clear display
	for (unsigned int i = 0; i < sizeof(io.keys) / sizeof(io.keys[0]); ++i) io.keys[i] = 0; // Clear keys
	for (unsigned int i = 0; i < sizeof(cpu.stack) / sizeof(cpu.stack[0]); ++i) cpu.stack[i] = 0; // Clear stack
	for (unsigned int i = 0; i < sizeof(cpu.V) / sizeof(cpu.V[0]); ++i) cpu.V[i] = 0; // Clear registers V0-VF
	for (unsigned int i = 0; i < sizeof(cpu.memory) / sizeof(cpu.memory[0]); ++i) cpu.memory[i] = 0; // Clear memory
	
	for (unsigned int i = 0; i < sizeof(fontset) / sizeof(fontset[0]); ++i) cpu.memory[i] = fontset[i]; // Load fontset into memory

	// Reset timers
	timer.delayTimer = 0;
	timer.soundTimer = 0;

	srand(seed); // Initialise random number generator

}

void Chip8::loadROM(ROM rom)
{
	for (unsigned int i = 0; i < rom.buffer.size(); ++i)
		cpu.memory[i + 0x200] = rom.buffer[i]; // Program data starts at 0x200 == 512
}

void Chip8::doEmulationCycle()
{
	// Fetch opcode - each is 2 bytes long so merge them
	cpu.opcode = cpu.memory[cpu.programCounter] << 8 | cpu.memory[cpu.programCounter + 1]; // 0xA2 + 0xF0 = 0xA2F0

	//printf ("Opcode [0x0000]: 0x%X\n", cpu.opcode & 0xF000);   

	uint8_t X = (cpu.opcode & 0x0F00) >> 8;
	uint8_t Y = (cpu.opcode & 0x00F0) >> 4;
	uint16_t NNN = cpu.opcode & 0x0FFF;
	uint8_t NN = cpu.opcode & 0x00FF;

	switch (cpu.opcode & 0xF000) // Rely on first 4 bits (big endian)
	{
		
		case 0x0000: 
			switch(cpu.opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clears the screen
					for (unsigned int x = 0; x < 64*32; ++x) io.pixels[x] = 0;
					cpu.drawFlag = true;
					cpu.programCounter += 2;
				break;
		
				case 0x000E: // 0x00EE: Returns from subroutine
					--cpu.stackPointer;  
					cpu.programCounter = cpu.stack[cpu.stackPointer];
					cpu.programCounter += 2;
				break;
			
				default:
					printf ("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);     
				break;     
			}
		break;

		case 0x1000: // 0x1NNN: Jumps to address NNN
			cpu.programCounter = NNN;
		break;

		case 0x2000: // 0x2NNN: Calls subroutine at NNN
			cpu.stack[cpu.stackPointer] = cpu.programCounter;
			++cpu.stackPointer;
			cpu.programCounter = NNN;
		break;	

		case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN

			if (cpu.V[X] == (NN))
				cpu.programCounter += 4;
			else
				cpu.programCounter += 2;

		break;

		case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN

			if (cpu.V[X] != NN)
				cpu.programCounter += 4;
			else
				cpu.programCounter += 2;

		break;

		case 0x5000: // 0x5XY0: Skips the next instruction if VX equals VY

			if (cpu.V[X] == cpu.V[Y])
				cpu.programCounter += 4;
			else
				cpu.programCounter += 2;

		break;

		case 0x6000: // 0x6XNN: Setx VX to NN

			cpu.V[X] = NN;
			cpu.programCounter += 2;

		break;

		case 0x7000: // 0x7XNN: Adds NN to VX

			cpu.V[X] += NN;
			cpu.programCounter += 2;

		break;

		case 0x8000:

			switch (cpu.opcode & 0x000F)
			{
				case 0x0000: // 0x8XY0: Sets VX to the value of VY                                               
					cpu.V[X] = cpu.V[Y];
					cpu.programCounter += 2;
				break;

				case 0x0001: // 0x8XY1: Sets VX to VX or VY (Bitwise OR operation)                                               
					cpu.V[X] |= cpu.V[Y];
					cpu.programCounter += 2;
				break;

				case 0x0002: // 0x8XY2: Sets VX to VX and VY (Bitwise AND operation)                                               
					cpu.V[X] &= cpu.V[Y];
					cpu.programCounter += 2;
				break;

				case 0x0003: // 0x8XY3: Sets VX to VX xor VY (Bitwise XOR operation)                                               
					cpu.V[X] ^= cpu.V[Y];
					cpu.programCounter += 2;
				break;
				
				case 0x0004: // 0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't                                               
					cpu.V[X] += cpu.V[Y];
					if (cpu.V[Y] > (0xFF - cpu.V[X]))
						cpu.V[0xF] = 1; //carry
					else
						cpu.V[0xF] = 0;
					cpu.programCounter += 2;
				break;

				case 0x0005: // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.                                      
					if (cpu.V[Y] > cpu.V[X])
						cpu.V[0xF] = 0; // there is a borrow
					else
						cpu.V[0xF] = 1;
					cpu.V[X] -= cpu.V[Y];
					cpu.programCounter += 2;
				break;

				case 0x0006: // 0x8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.                                
					cpu.V[0xF] = cpu.V[X] & 0x1;
					cpu.V[X] >>= 1;
					cpu.programCounter += 2;
				break;

				case 0x0007: // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.            
					if (cpu.V[X] > cpu.V[Y])	// VY-VX
						cpu.V[0xF] = 0; // there is a borrow
					else
						cpu.V[0xF] = 1;
					cpu.V[X] = cpu.V[Y] - cpu.V[X];
					cpu.programCounter += 2;
				break;

				case 0x000E: // 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					cpu.V[0xF] = cpu.V[X] >> 7;
					cpu.V[X] <<= 1;
					cpu.programCounter += 2;
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);
				break;
			}

		break;

		case 0x9000: // 0x9XY0: Skips the next instruction if VX doesn't equal VY.
			
			if (cpu.V[X] != cpu.V[Y])
				cpu.programCounter += 4;
			else
				cpu.programCounter += 2;
		
		break;

		case 0xA000: // 0xANNN: Sets I to the address NNN
			cpu.indexRegister = NNN;
			cpu.programCounter += 2;
		break;
		
		case 0xB000: // 0xBNNN: Jumps to the address NNN plus V0
			cpu.programCounter = NNN + cpu.V[0];
		break;

		case 0xC000: // 0xCXNN: Sets VX to the result of a bitwise and operation on a random number (typically 0 to 255) and NN
			cpu.V[X] = (rand() % 256) & NN;
			cpu.programCounter += 2;
		break;

		case 0xD000:
		{
			unsigned short x = cpu.V[X];
			unsigned short y = cpu.V[Y];
			unsigned short height = cpu.opcode & 0x000F;
			unsigned short pixel;

			cpu.V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = cpu.memory[cpu.indexRegister + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						if (io.pixels[(x + xline + ((y + yline) * 64))] == 1)
						{
							cpu.V[0xF] = 1;
						}
						io.pixels[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			cpu.drawFlag = true;
			cpu.programCounter += 2;
		}
		break;

		case 0xE000:

			switch (cpu.opcode & 0x000F)
			{
				case 0x000E: // 0xEX9E: Skips the next instruction if the key stored in VX is pressed
					if (io.keys[cpu.V[X]] != 0)
						cpu.programCounter += 4;
					else
						cpu.programCounter += 2;
				break;

				case 0x0001: // 0xEXA1: Skips the next instruction if the key stored in VX isn't pressed
					if (io.keys[cpu.V[X]] == 0)
						cpu.programCounter += 4;
					else
						cpu.programCounter += 2;
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);
				break;
			}

		break;

		case 0xF000:

			switch (cpu.opcode & 0x000F)
			{
				case 0x0007: // 0xFX07: Sets VX to the value of the delay timer
					cpu.V[X] = timer.delayTimer;
					cpu.programCounter += 2;
				break;

				case 0x000A: // 0xFX0A: A key press is awaited, and then stored in VX (all instructions halted)
				{
					bool keyPressed = false;

					std::cout << "Awaiting key press..." << std::endl;
					while (!keyPressed)
					{
						for (unsigned int i = 0; i < sizeof(io.keys) / sizeof(io.keys[0]); ++i)
						{
							if (io.keys[i] != 0)
							{
								cpu.V[X] = i;
								keyPressed = true;
							}
						}
					}

					cpu.programCounter += 2;

					break;
				}

				case 0x0005:
					switch (cpu.opcode & 0x00F0)
					{

						case 0x0010: // 0xFX15: Sets the delay timer to VX
						
							timer.delayTimer = cpu.V[X];
							cpu.programCounter += 2;

						break;

						case 0x0050: // 0xFX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.

							for (unsigned int i = 0; i <= X; ++i) cpu.memory[cpu.indexRegister + i] = cpu.V[i];

							// On the original interpreter, when the operation is done, I = I + X + 1.
							cpu.indexRegister += X + 1;

							cpu.programCounter += 2;

						break;

						case 0x0060: // 0xFX65: Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.

							for (unsigned int i = 0; i <= X; ++i) cpu.V[i] = cpu.memory[cpu.indexRegister + i];

							// On the original interpreter, when the operation is done, I = I + X + 1.
							cpu.indexRegister += X + 1;

							cpu.programCounter += 2;

						break;

						default:
							printf("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);
						break;
					}
				break;

				case 0x0008: // 0xFX18: Sets the sound timer to VX

					timer.soundTimer = cpu.V[X];
					cpu.programCounter += 2;

				break;

				case 0x000E: // 0xFX1E: Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.

					cpu.indexRegister += cpu.V[X];
					if (cpu.indexRegister > 0xFFF) cpu.V[0xF] = 1;
					else cpu.V[0xF] = 0;

					cpu.programCounter += 2;

				break;

				case 0x0009: // 0xFX29: Sets I to the location of the sprite of the character in VX. Characters 0-F (in hex) are represented by a 4x5 font.

					// Fonts exist in memory from 0x0 to 0x50 (80), with each character taking up 5 bytes
					cpu.indexRegister = cpu.V[X] * 5;
					cpu.programCounter += 2;
				
				break;

				case 0x0003: // 0xFX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, 
				{			 // the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX,
							 // place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)

					int hundreds = (cpu.V[X] / 100);
					int tens = (cpu.V[X] / 10) % 10;
					int units = cpu.V[X] % 10;

					cpu.memory[cpu.indexRegister] = hundreds;
					cpu.memory[cpu.indexRegister + 1] = tens;
					cpu.memory[cpu.indexRegister + 2] = units;

					cpu.programCounter += 2;

					break;
				}

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);
				break;
			}

		break;

		default:
			printf ("Unknown opcode [0x0000]: 0x%X\n", cpu.opcode);  
			cpu.programCounter += 2;
		break;  
	}

	if (timer.delayTimer > 0) timer.delayTimer--;
	if (timer.soundTimer > 0) timer.soundTimer--;

}