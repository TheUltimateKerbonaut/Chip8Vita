#ifndef ROM_H
#define ROM_H

#include <string>
#include <vector>

#include <psp2/types.h> 

class ROM
{

public:

	ROM(std::string fileName, SceOff size);

	std::vector<unsigned char> buffer = std::vector<unsigned char>();

};

#endif