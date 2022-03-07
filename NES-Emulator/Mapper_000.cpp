#include "Mapper_000.hpp"

const unsigned int MIN_CPU_MAP = 0x8000;
const unsigned int MAX_CPU_MAP = 0xFFFF;
const unsigned int MIN_PPU_MAP = 0x0000;
const unsigned int MAX_PPU_MAP = 0x1FFF;

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
}

Mapper_000::~Mapper_000()
{
}

bool Mapper_000::cpuMapRead(uint16_t address, uint32_t& mapped_address)
{
    if (address >= MIN_CPU_MAP && address <= MAX_CPU_MAP)
    {
        mapped_address = address & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t address, uint32_t& mapped_address)
{
    if (address >= MIN_CPU_MAP && address <= MAX_CPU_MAP)
    {
        mapped_address = address & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapRead(uint16_t address, uint32_t& mapped_address)
{
    if (address >= MIN_PPU_MAP && address <= MAX_PPU_MAP)
    {
        mapped_address = address;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t address, uint32_t& mapped_address)
{
    return false;
}
