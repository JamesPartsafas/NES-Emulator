#include "Bus.hpp"

const unsigned int MIN_ADDRESS = 0x0000;
const unsigned int MAX_ADDRESS = 0xFFFF;

Bus::Bus()
{
	//Connect CPU to Bus
	cpu.ConnectBus(this);
	
	//Clear RAM contents
	for (auto &i : ram)
		i = 0x00;
}

Bus::~Bus()
{

}

void Bus::write(uint16_t address, uint8_t data)
{
	if (address >= MIN_ADDRESS && address <= MAX_ADDRESS)
	{
		ram[address] = data;
	}
}

uint8_t Bus::read(uint16_t address, bool bReadOnly)
{
	if (address >= MIN_ADDRESS && address <= MAX_ADDRESS)
	{
		return ram[address];
	}

	return 0x00;
}