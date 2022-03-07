#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

#include "Mapper_000.hpp"

class Cartridge
{
public:
	//Constructors/Destructors
	Cartridge(const std::string& sFileName);
	~Cartridge();

	bool ImageValid();

	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

	//Communicate with main bus
	bool cpuRead(uint16_t address, uint8_t &data);
	bool cpuWrite(uint16_t address, uint8_t data);

	//Communicate with internal PPU Bus
	bool ppuRead(uint16_t address, uint8_t &data);
	bool ppuWrite(uint16_t address, uint8_t data);

private:
	bool bImageValid = false;

	std::vector<uint8_t> vPRGMemory; //Program memory
	std::vector<uint8_t> vCHRMemory; //Character/graphics memory

	uint8_t nMapperID = 0;
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;

	std::shared_ptr<Mapper> pMapper;
};