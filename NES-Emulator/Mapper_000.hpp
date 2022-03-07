#pragma once
#include "Mapper.hpp"

class Mapper_000 : public Mapper
{
public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper_000();

	// Inherited via Mapper
	virtual bool cpuMapRead(uint16_t address, uint32_t& mapped_address) override;
	virtual bool cpuMapWrite(uint16_t address, uint32_t& mapped_address) override;
	virtual bool ppuMapRead(uint16_t address, uint32_t& mapped_address) override;
	virtual bool ppuMapWrite(uint16_t address, uint32_t& mapped_address) override;
};

