#include "Bus.hpp"

const unsigned int MIN_CPU_ADDRESS = 0x0000;
const unsigned int MAX_CPU_ADDRESS = 0x1FFF;
const unsigned int MIN_PPU_ADDRESS = 0x2000;
const unsigned int MAX_PPU_ADDRESS = 0x3FFF;

Bus::Bus()
{
	//Connect CPU to Bus
	cpu.ConnectBus(this);
	
	//Clear RAM contents
	for (auto &i : cpuRam)
		i = 0x00;
}

Bus::~Bus()
{

}

void Bus::cpuWrite(uint16_t address, uint8_t data)
{
	if (cartridge->cpuWrite(address, data))
	{
		//Writing handled internally in Cartridge
	}
	else if (address >= MIN_CPU_ADDRESS && address <= MAX_CPU_ADDRESS)
	{
		cpuRam[address & 0x07FF] = data;
	}
	else if (address >= MIN_PPU_ADDRESS && address <= MAX_PPU_ADDRESS)
	{
		ppu.cpuWrite(address & 0x0007, data);
	}
}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly)
{
	uint8_t data = 0x00;

	if (cartridge->cpuRead(address, data))
	{
		//Reading handled internally in Cartridge and assigned to data
	}
	else if (address >= MIN_CPU_ADDRESS && address <= MAX_CPU_ADDRESS)
	{
		data = cpuRam[address & 0x07FF]; //Handle RAM mirroring
	}
	else if (address >= MIN_PPU_ADDRESS && address <= MAX_PPU_ADDRESS)
	{
		data = ppu.cpuRead(address & 0x0007, bReadOnly);
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cartridge = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();

	//Run a cpu clock every 3 ppu clocks
	if (nSystemClockCounter % 3 == 0)
	{
		cpu.clock();
	}
	nSystemClockCounter++;
}
