#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.hpp"
#include "olcPixelGameEngine.h"

const unsigned int NAME_TABLE_LENGTH = 1024;
const unsigned int PALETTE_TABLE_LENGTH = 32;
const unsigned int PATTERN_TABLE_LENGTH = 4096;

class Ppu2C02
{
public:
	//Constructors/Destructors
	Ppu2C02();
	~Ppu2C02();

	//Communicate with main bus
	uint8_t cpuRead(uint16_t address, bool rdonly = false);
	void cpuWrite(uint16_t address, uint8_t data);

	//Communicate with internal PPU Bus
	uint8_t ppuRead(uint16_t address, bool rdonly = false);
	void ppuWrite(uint16_t address, uint8_t data);

	//Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();

	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	std::shared_ptr<Cartridge> cartridge;
	uint8_t tblName[2][NAME_TABLE_LENGTH];
	uint8_t tblPalette[PALETTE_TABLE_LENGTH];
	uint8_t tblPattern[2][PATTERN_TABLE_LENGTH];

	int16_t scanline = 0;
	int16_t cycle = 0;

	//Color data
	olc::Pixel  palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };
};