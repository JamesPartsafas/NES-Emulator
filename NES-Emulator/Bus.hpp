#pragma once
#include <cstdint>
#include <array>
#include "Cpu6502.hpp"

const unsigned int MEMORY_SIZE = 64 * 1024;

class Bus
{
public:
	//Constructors/Destructors
	Bus();
	~Bus();

	//Devices on bus
	Cpu6502 cpu;
	std::array<uint8_t, MEMORY_SIZE> ram;

	//Read and write to bus addresses
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool bReadOnly = false);
};