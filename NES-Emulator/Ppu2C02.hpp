#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.hpp"
#include "olcPixelGameEngine.h"

const unsigned int NAME_TABLE_LENGTH = 1024;
const unsigned int PALETTE_TABLE_LENGTH = 32;
const unsigned int PATTERN_TABLE_SECTION = 16;
const unsigned int PATTERN_TABLE_LENGTH = 4096;
const unsigned int PATTERN_TABLE_START = 0x0000;
const unsigned int PATTERN_TABLE_END = 0x1FFF;
const unsigned int NAME_TABLE_START = 0x2000;
const unsigned int NAME_TABLE_END = 0x3EFF;
const unsigned int PALETTE_TABLE_START = 0x3F00;
const unsigned int PALETTE_TABLE_END = 0x3FFF;
const unsigned int VERTICAL_BLANK_START = 241;
const unsigned int MIRROR_MAPPING_LIMIT = 0x03FF;
const unsigned int PPU_ADDRESSABLE_RANGE = 0x3F00;

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
	bool nmi = false;
	void reset();

	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);
	olc::Pixel& GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
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

	//Data registers
	union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t data;
	} statusRegister;

	union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t data;
	} maskRegister;

	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};

		uint8_t data;
	} controlRegister;

	union loopyRegister
	{
		struct
		{

			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t dataRegister = 0x0000;
	};

	loopyRegister vRamAddress;
	loopyRegister tRamAddress;

	uint8_t fineX = 0x00;

	// Background registers
	uint8_t bgNextTileId = 0x00;
	uint8_t bgNextTileAttribute = 0x00;
	uint8_t bgNextTileLsb = 0x00;
	uint8_t bgNextTileMsb = 0x00;

	uint16_t bgShiftPatternLow = 0x0000;
	uint16_t bgShiftPatternHigh = 0x0000;
	uint16_t bgShiftAttributeLow = 0x0000;
	uint16_t bgShiftAttributeHigh = 0x0000;

	// Internal
	uint8_t addressLatch = 0x00; //Mark low or high part of byte
	uint8_t ppuDataBuffer = 0x00; //Handle PPU delay relative to CPU
};