#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "Cpu6502.hpp"
#include "Ppu2C02.hpp"
#include "Cartridge.hpp"

const unsigned int MEMORY_SIZE = 2048;

class Bus
{
public:
	//Constructors/Destructors
	Bus();
	~Bus();

	//Devices on bus
	Cpu6502 cpu;
	Ppu2C02 ppu;
	std::array<uint8_t, MEMORY_SIZE> cpuRam;
	std::shared_ptr<Cartridge> cartridge;

	//Read and write to bus addresses
	void cpuWrite(uint16_t address, uint8_t data);
	uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

	//System interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	uint32_t nSystemClockCounter = 0;
};