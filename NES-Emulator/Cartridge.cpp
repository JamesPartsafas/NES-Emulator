#include "Cartridge.hpp"

const unsigned int PRG_BANK_LENGTH = 16384;
const unsigned int CHR_BANK_LENGTH = 8192;

Cartridge::Cartridge(const std::string& sFileName)
{
	// Format Header in iNES format
	struct sHeader
	{
		char name[4];
		uint8_t prg_rom_chunks;
		uint8_t chr_rom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
	} header;

	bImageValid = false;

	//Read ROM
	std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary);
	if (ifs.is_open())
	{
		//Read header
		ifs.read((char*)&header, sizeof(sHeader));

		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		//Determine ID of mapper
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		//Determine file format
		uint8_t nFileType = 1;

		if (nFileType == 0)
		{
			//Placeholder
		}

		if (nFileType == 1)
		{
			nPRGBanks = header.prg_rom_chunks;
			vPRGMemory.resize(nPRGBanks * PRG_BANK_LENGTH);
			ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

			nCHRBanks = header.chr_rom_chunks;
			vCHRMemory.resize(nCHRBanks * CHR_BANK_LENGTH);
			ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
		}

		if (nFileType == 2)
		{
			//Placeholder
		}

		//Load mapper
		switch (nMapperID)
		{
		case 0: 
			pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
			break;
		}

		bImageValid = true;
		ifs.close();
	}
}

Cartridge::~Cartridge()
{
}

bool Cartridge::ImageValid()
{
	return bImageValid;
}

bool Cartridge::cpuRead(uint16_t address, uint8_t &data)
{
	uint32_t mapped_address = 0;
	if (pMapper->cpuMapRead(address, mapped_address))
	{
		data = vPRGMemory[mapped_address];
		return true;
	}

	return false;
}

bool Cartridge::cpuWrite(uint16_t address, uint8_t data)
{
	uint32_t mapped_address = 0;
	if (pMapper->cpuMapWrite(address, mapped_address))
	{
		vPRGMemory[mapped_address] = data;
		return true;
	}

	return false;
}

bool Cartridge::ppuRead(uint16_t address, uint8_t &data)
{
	uint32_t mapped_address = 0;
	if (pMapper->ppuMapRead(address, mapped_address))
	{
		data = vCHRMemory[mapped_address];
		return true;
	}

	return false;
}

bool Cartridge::ppuWrite(uint16_t address, uint8_t data)
{
	uint32_t mapped_address = 0;
	if (pMapper->ppuMapWrite(address, mapped_address))
	{
		vCHRMemory[mapped_address] = data;
		return true;
	}

	return false;
}
