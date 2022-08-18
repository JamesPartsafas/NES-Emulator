#include "Ppu2C02.hpp"

Ppu2C02::Ppu2C02()
{
	//Available colors
	palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

Ppu2C02::~Ppu2C02()
{
}

uint8_t Ppu2C02::cpuRead(uint16_t address, bool readonly)
{
	uint8_t data = 0x00;

	if (readonly)
	{
		switch (address)
		{
		case 0x0000: // Control
			data = controlRegister.data;
			break;
		case 0x0001: // Mask
			data = maskRegister.data;
			break;
		case 0x0002: // Status
			data = statusRegister.data;
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			break;
		}
	}
	else
	{
		switch (address)
		{
		case 0x0000: // Control
			break;
		case 0x0001: // Mask
			break;
		case 0x0002: // Status
			data = (statusRegister.data & 0xE0) | (ppuDataBuffer & 0x1F);
			statusRegister.vertical_blank = 0;
			addressLatch = 0;
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			data = ppuDataBuffer;
			ppuDataBuffer = ppuRead(vRamAddress.dataRegister);

			//Handle reading from palette table
			if (vRamAddress.dataRegister > 0x3F00)
				data = ppuDataBuffer;

			vRamAddress.dataRegister += (controlRegister.increment_mode ? 32 : 1);
			break;
		}
	}

	return data;
}

void Ppu2C02::cpuWrite(uint16_t address, uint8_t data)
{
	switch (address)
	{
		case 0x0000: // Control
			controlRegister.data = data;
			tRamAddress.nametable_x = controlRegister.nametable_x;
			tRamAddress.nametable_y = controlRegister.nametable_y;
			break;
		case 0x0001: // Mask
			maskRegister.data = data;
			break;
		case 0x0002: // Status
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			if (addressLatch == 0)
			{
				fineX = data & 0x07;
				tRamAddress.coarse_x = data >> 3;
				addressLatch = 1;
			}
			else
			{
				tRamAddress.fine_y = data & 0x07;
				tRamAddress.coarse_y = data >> 3;
				addressLatch = 0;
			}
			break;
		case 0x0006: // PPU Address
			if (addressLatch == 0)
			{
				tRamAddress.dataRegister = (uint16_t)((data & 0x3F) << 8) | (tRamAddress.dataRegister & 0x00FF);
				addressLatch = 1;
			}
			else
			{
				tRamAddress.dataRegister = (tRamAddress.dataRegister & 0xFF00) | data;
				vRamAddress = tRamAddress;
				addressLatch = 0;
			}
			break;
		case 0x0007: // PPU Data
			ppuWrite(vRamAddress.dataRegister, data);
			vRamAddress.dataRegister += (controlRegister.increment_mode ? 32 : 1);
			break;
	}
}

uint8_t Ppu2C02::ppuRead(uint16_t address, bool rdonly)
{
	uint8_t data = 0x00;
	address &= 0x3FFF;

	if (cartridge->ppuRead(address, data))
	{

	}
	else if (address >= PATTERN_TABLE_START && address <= PATTERN_TABLE_END)
	{
		//Used if cartridge cannot map the address
		data = tblPattern[(address & 0x1000) >> 12][address & 0x0FFF];
	}
	else if (address >= NAME_TABLE_START && address <= NAME_TABLE_END)
	{
		address &= 0x0FFF;

		if (cartridge->mirror == Cartridge::MIRROR::VERTICAL)
		{
			//Vertical mirroring mapping
			if (address >= 0x0000 && address <= MIRROR_MAPPING_LIMIT)
				data = tblName[0][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0400 && address <= 0x07FF)
				data = tblName[1][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = tblName[0][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = tblName[1][address & MIRROR_MAPPING_LIMIT];
		}
		else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			//Horizontal mirroring mapping
			if (address >= 0x0000 && address <= MIRROR_MAPPING_LIMIT)
				data = tblName[0][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0400 && address <= 0x07FF)
				data = tblName[0][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = tblName[1][address & MIRROR_MAPPING_LIMIT];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = tblName[1][address & MIRROR_MAPPING_LIMIT];
		}
	}
	else if (address >= PALETTE_TABLE_START && address <= PALETTE_TABLE_END)
	{
		address &= 0x001F;
		if (address == 0x0010) 
			address = 0x0000;
		if (address == 0x0014) 
			address = 0x0004;
		if (address == 0x0018) 
			address = 0x0008;
		if (address == 0x001C) 
			address = 0x000C;

		data = tblPalette[address] & (maskRegister.grayscale ? 0x30 : 0x3F);
	}

	return data;
}

void Ppu2C02::ppuWrite(uint16_t address, uint8_t data)
{
	address &= 0x3FFF;

	if (cartridge->ppuWrite(address, data))
	{

	}
	else if (address >= PATTERN_TABLE_START && address <= PATTERN_TABLE_END)
	{
		//Used if cartridge cannot map the address
		tblPattern[(address & 0x1000) >> 12][address & 0x0FFF] = data;
	}
	else if (address >= NAME_TABLE_START && address <= NAME_TABLE_END)
	{
		if (cartridge->mirror == Cartridge::MIRROR::VERTICAL)
		{
			//Vertical mirroring mapping
			if (address >= 0x0000 && address <= MIRROR_MAPPING_LIMIT)
				tblName[0][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0400 && address <= 0x07FF)
				tblName[1][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0800 && address <= 0x0BFF)
				tblName[0][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0C00 && address <= 0x0FFF)
				tblName[1][address & MIRROR_MAPPING_LIMIT] = data;
		}
		else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			//Horizontal mirroring mapping
			if (address >= 0x0000 && address <= MIRROR_MAPPING_LIMIT)
				tblName[0][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0400 && address <= 0x07FF)
				tblName[0][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0800 && address <= 0x0BFF)
				tblName[1][address & MIRROR_MAPPING_LIMIT] = data;
			if (address >= 0x0C00 && address <= 0x0FFF)
				tblName[1][address & MIRROR_MAPPING_LIMIT] = data;
		}
	}
	else if (address >= PALETTE_TABLE_START && address <= PALETTE_TABLE_END)
	{
		address &= 0x001F;
		if (address == 0x0010)
			address = 0x0000;
		if (address == 0x0014)
			address = 0x0004;
		if (address == 0x0018)
			address = 0x0008;
		if (address == 0x001C)
			address = 0x000C;

		tblPalette[address] = data;
	}
}

olc::Sprite& Ppu2C02::GetScreen()
{
	return sprScreen;
}

olc::Sprite& Ppu2C02::GetNameTable(uint8_t i)
{
	return sprNameTable[i];
}

olc::Sprite& Ppu2C02::GetPatternTable(uint8_t i, uint8_t palette)
{
	//Loop through pattern table
	for (uint16_t nTileY = 0; nTileY < PATTERN_TABLE_SECTION; nTileY++)
	{
		for (uint16_t nTileX = 0; nTileX < PATTERN_TABLE_SECTION; nTileX++)
		{
			//Get 1D coordinate into pattern table
			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			//Move through tile
			for (uint16_t row = 0; row < 8; row++)
			{
				uint8_t tileLsb = ppuRead(i * PATTERN_TABLE_LENGTH + nOffset + row);
				uint8_t tileMsb = ppuRead(i * PATTERN_TABLE_LENGTH + nOffset + row + 8);

				for (uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (tileLsb & 0x01) + (tileMsb & 0x01);
					tileLsb >>= 1;
					tileMsb >>= 1;

					sprPatternTable[i].SetPixel(
						nTileX * 8 + (7 - col),
						nTileY * 8 + row,
						GetColorFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}

	return sprPatternTable[i];
}

olc::Pixel& Ppu2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	return palScreen[ppuRead(PPU_ADDRESSABLE_RANGE + (palette << 2) + pixel) & 0x3F];
}

void Ppu2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cartridge = cartridge;
}

void Ppu2C02::clock()
{
	auto IncrementScrollX = [&]()
	{
		if (maskRegister.render_background || maskRegister.render_sprites)
		{
			if (vRamAddress.coarse_x == 31)
			{
				vRamAddress.coarse_x = 0;
				vRamAddress.nametable_x = ~vRamAddress.nametable_x;
			}
			else
			{
				vRamAddress.coarse_x++;
			}
		}
	};

	auto IncrementScrollY = [&]()
	{
		if (maskRegister.render_background || maskRegister.render_sprites)
		{
			if (vRamAddress.fine_y < 7)
			{
				vRamAddress.fine_y++;
			}
			else
			{
				vRamAddress.fine_y = 0;

				if (vRamAddress.coarse_y == 29)
				{
					vRamAddress.coarse_y = 0;
					vRamAddress.nametable_y = ~vRamAddress.nametable_y;
				}
				else if (vRamAddress.coarse_y == 31)
				{
					vRamAddress.coarse_y = 0;
				}
				else
				{
					vRamAddress.coarse_y++;
				}
			}
		}
	};

	auto transferAddressX = [&]()
	{
		if (maskRegister.render_background || maskRegister.render_sprites)
		{
			vRamAddress.nametable_x = tRamAddress.nametable_x;
			vRamAddress.coarse_x = tRamAddress.coarse_x;
		}
	};

	auto transferAddressY = [&]()
	{
		if (maskRegister.render_background || maskRegister.render_sprites)
		{
			vRamAddress.fine_y = tRamAddress.fine_y;
			vRamAddress.nametable_y = tRamAddress.nametable_y;
			vRamAddress.coarse_y = tRamAddress.coarse_y;
		}
	};

	auto LoadBackgroundShiftRegisters = [&]()
	{
		bgShiftPatternLow = (bgShiftPatternLow & 0xFF00) | bgNextTileLsb;
		bgShiftPatternHigh = (bgShiftPatternHigh & 0xFF00) | bgNextTileMsb;
		bgShiftAttributeLow = (bgShiftAttributeLow & 0xFF00) | ((bgNextTileAttribute & 0b01) ? 0xFF : 0x00);
		bgShiftAttributeHigh = (bgShiftAttributeHigh & 0xFF00) | ((bgNextTileAttribute & 0x10) ? 0xFF : 0x00);
	};

	auto UpdateShiftRegisters = [&]()
	{
		if (maskRegister.render_background)
		{
			bgShiftPatternLow <<= 1;
			bgShiftPatternHigh <<= 1;
			bgShiftAttributeLow <<= 1;
			bgShiftAttributeHigh <<= 1;
		}
	};

	if (scanline >= -1 && scanline < (VERTICAL_BLANK_START - 1))
	{
		if (scanline == -1 && cycle == 1)
		{
			statusRegister.vertical_blank = 0;
		}

		// Visible cycle
		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			UpdateShiftRegisters();

			switch ((cycle - 1) % 8)
			{
			case 0:
				LoadBackgroundShiftRegisters();
				bgNextTileId = ppuRead(0x2000 | (vRamAddress.dataRegister & 0x0FFF));
				break;
			case 2:
				bgNextTileAttribute = ppuRead(0x23C0 | (vRamAddress.nametable_y << 11)
					| (vRamAddress.nametable_x << 10)
					| ((vRamAddress.coarse_y >> 2) << 3)
					| (vRamAddress.coarse_x >> 2));

				if (vRamAddress.coarse_y & 0x02)
					bgNextTileAttribute >>= 4;

				if (vRamAddress.coarse_x & 0x02)
					bgNextTileAttribute >>= 2;

				bgNextTileAttribute &= 0x03;
				break;
			case 4:
				bgNextTileLsb = ppuRead((controlRegister.pattern_background << 12)
					+ ((uint16_t)bgNextTileId << 4)
					+ (vRamAddress.fine_y));
				break;
			case 6:
				bgNextTileMsb = ppuRead((controlRegister.pattern_background << 12)
					+ ((uint16_t)bgNextTileId << 4)
					+ (vRamAddress.fine_y) + 8);
				break;
			case 7:
				IncrementScrollX();
				break;
			}
		}

		if (cycle == 256)
		{
			IncrementScrollY();
		}

		if (cycle == 257)
		{
			transferAddressX();
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			transferAddressY();
		}
	}

	if (scanline == (VERTICAL_BLANK_START - 1))
	{
		// Do nothing. In post-render line
	}

	if (scanline == VERTICAL_BLANK_START && cycle == 1)
	{
		statusRegister.vertical_blank = 1;
		if (controlRegister.enable_nmi)
			nmi = true;
	}

	//Draw current pixel
	uint8_t bgPixel = 0x00;
	uint8_t bgPalette = 0x00;

	if (maskRegister.render_background)
	{
		uint16_t bitMux = 0x8000 >> fineX;

		uint8_t p0Pixel = (bgShiftPatternLow & bitMux) > 0;
		uint8_t p1Pixel = (bgShiftPatternHigh & bitMux) > 0;
		bgPixel = (p1Pixel << 1) | p0Pixel;

		uint8_t bgPalette0 = (bgShiftAttributeLow & bitMux) > 0;
		uint8_t bgPalette1 = (bgShiftAttributeHigh & bitMux) > 0;
		bgPalette = (bgPalette1 << 1) | bgPalette0;
	}

	sprScreen.SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(bgPalette, bgPixel));

	//Advance renderer
	cycle++;
	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;

		if (scanline >= 261)
		{
			scanline = -1;
			frame_complete = true;
		}
	}
}

void Ppu2C02::reset()
{
	fineX = 0x00;
	addressLatch = 0x00;
	ppuDataBuffer = 0x00;
	scanline = 0;
	cycle = 0;
	bgNextTileId = 0x00;
	bgNextTileAttribute = 0x00;
	bgNextTileLsb = 0x00;
	bgNextTileMsb = 0x00;
	bgShiftPatternLow = 0x0000;
	bgShiftPatternHigh = 0x0000;
	bgShiftAttributeLow = 0x0000;
	bgShiftAttributeHigh = 0x0000;
	statusRegister.data = 0x00;
	maskRegister.data = 0x00;
	controlRegister.data = 0x00;
	vRamAddress.dataRegister = 0x0000;
	tRamAddress.dataRegister = 0x0000;
}
