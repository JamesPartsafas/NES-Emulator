#include "Cpu6502.hpp"
#include "Bus.hpp"

const unsigned int STACK_POINTER_BASE_LOCATION = 0x0100;
const unsigned int PROGRAM_DATA_BASE_LOCATION = 0xFFFC;
const unsigned int INTERRUPT_SERVICE_BASE_LOCATION = 0xFFFE;
const unsigned int NM_INTERRUPT_SERVICE_BASE_LOCATION = 0xFFFA;
const unsigned int MAX_ADDRESS = 0xFFFF;

Cpu6502::Cpu6502()
{
	//Assign opcodes to related functions
	using a = Cpu6502;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

Cpu6502::~Cpu6502()
{

}

//Bus connection functionality
uint8_t Cpu6502::read(uint16_t address)
{
	return bus->cpuRead(address, false);
}

void Cpu6502::write(uint16_t address, uint8_t data)
{
	bus->cpuWrite(address, data);
}

//Inputs
void Cpu6502::clock()
{
	//Execute opcode then wait until set number of clock cycles have passed
	if (cycles == 0)
	{
		opcode = read(pc);
		pc++;

		cycles = lookup[opcode].cycles;
		uint8_t additionalAddrCycle = (this->*lookup[opcode].addrmode)();
		uint8_t additionalOpCycle = (this->*lookup[opcode].operate)();

		cycles += (additionalAddrCycle & additionalOpCycle);
	}

	cycles--;
}

void Cpu6502::reset()
{
	a = 0;
	x = 0;
	y = 0;
	sp = 0xFD;
	status = 0x00 | U;

	addr_abs = PROGRAM_DATA_BASE_LOCATION;
	uint16_t low = read(addr_abs);
	uint16_t high = read(addr_abs + 1);
	pc = (high << 8) | low;

	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	cycles = 8;
}


//Interrupt request
//Save state of program before servicing interrupt
void Cpu6502::irq()
{
	if (GetFlag(I) == 0)
	{
		write(STACK_POINTER_BASE_LOCATION + sp, (pc >> 8) & 0x00FF);
		sp--;
		write(STACK_POINTER_BASE_LOCATION + sp, pc & 0x00FF);
		sp--;

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(STACK_POINTER_BASE_LOCATION + sp, status);
		sp--;

		addr_abs = INTERRUPT_SERVICE_BASE_LOCATION;
		uint16_t low = read(addr_abs);
		uint16_t high = read(addr_abs + 1);
		pc = (high << 8) | low;

		cycles = 7;
	}
}

//Non-maskable interrupt request
//Save state of program before servicing interrupt
void Cpu6502::nmi()
{
	write(STACK_POINTER_BASE_LOCATION + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(STACK_POINTER_BASE_LOCATION + sp, pc & 0x00FF);
	sp--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(STACK_POINTER_BASE_LOCATION + sp, status);
	sp--;

	addr_abs = NM_INTERRUPT_SERVICE_BASE_LOCATION;
	uint16_t low = read(addr_abs);
	uint16_t high = read(addr_abs + 1);
	pc = (high << 8) | low;

	cycles = 8;
}

//Flag handling
uint8_t Cpu6502::GetFlag(FLAGS6502 flag)
{
	return ((status & flag) > 0) ? 1 : 0;
}

void Cpu6502::SetFlag(FLAGS6502 flag, bool value)
{
	if (value)
	{
		status |= flag;
	}
	else
	{
		status &= ~flag;
	}
}

//Addressing modes
#pragma region

//Implied (No address required)
uint8_t Cpu6502::IMP()
{
	fetched = a;
	return 0;
}

//Immediate (address is the pc value)
uint8_t Cpu6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

//Zero-page addressing (Address from page 0 with an offset)
uint8_t Cpu6502::ZP0()
{
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

//Zero-page addressing with X offset
uint8_t Cpu6502::ZPX()
{
	addr_abs = read(pc) + x;
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

//Zero-page addressing with Y offset
uint8_t Cpu6502::ZPY()
{
	addr_abs = read(pc) + y;
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

//Absolute addressing (address is specified in next 2 bytes)
uint8_t Cpu6502::ABS()
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_abs = (high << 8) | low;

	return 0;
}

//Absolute addressing with X offset
//If offset causes a page switch, an extra clock cycles is needed
uint8_t Cpu6502::ABX()
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_abs = (high << 8) | low;
	addr_abs += x;

	if ((addr_abs & 0xFF00) != (high << 8))
		return 1;

	return 0;
}

//Absolute addressing with Y offset
uint8_t Cpu6502::ABY()
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_abs = (high << 8) | low;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (high << 8))
		return 1;

	return 0;
}

//Indirect addressing
//Due to a hardware bug, a pointer to a page boundary
//has the high bit read from the start of the page
uint8_t Cpu6502::IND()
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	uint16_t pointer = (high << 8) | low;

	if (low == 0x00FF) //Hardware bug
	{
		addr_abs = (read(pointer & 0xFF00) << 8) | read(pointer);
	}
	else //Normal behaviour
	{
		addr_abs = (read(pointer + 1) << 8) | read(pointer);
	}

	return 0;
}

//Indirect addressing of 0 page with X offset
uint8_t Cpu6502::IZX()
{
	uint16_t offset = read(pc);
	pc++;

	uint16_t low = read((offset + (uint16_t)x) & 0x00FF);
	uint16_t high = read((offset + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (high << 8) | low;

	return 0;
}

//Indirect addressing of 0 page with Y offset
//Page switches due to offset require an extra clock cycle
uint8_t Cpu6502::IZY()
{
	uint16_t offset = read(pc);
	pc++;

	uint16_t low = read(offset & 0x00FF);
	uint16_t high = read((offset + 1) & 0x00FF);

	addr_abs = (high << 8) | low;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (high << 8))
		return 1;

	return 0;
}

//Relative addressing mode
uint8_t Cpu6502::REL()
{
	addr_rel = read(pc);
	pc++;

	//Check for negative relative address
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;

	return 0;
}

#pragma endregion

//Opcodes
#pragma region

uint8_t Cpu6502::fetch()
{
	if (lookup[opcode].addrmode != &Cpu6502::IMP)
		fetched = read(addr_abs);

	return fetched;
}

//ADD fetched to accumulator
uint8_t Cpu6502::ADC()
{
	fetch();

	uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

	SetFlag(C, temp > 255);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, (temp & 0x80));
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

	a = temp & 0x00FF;

	return 1;
}

//AND on accumulator and fetched value
uint8_t Cpu6502::AND()
{
	fetch();
	a &= fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 1;
}

//Shift left
uint8_t Cpu6502::ASL()
{
	fetch();
	uint16_t temp = (uint16_t)fetched << 1;

	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);

	if (lookup[opcode].addrmode == &Cpu6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);

	return 0;
}

//Branch if carry bit is set
uint8_t Cpu6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Branch if carry bit is not set
uint8_t Cpu6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Branch if zero bit is set
uint8_t Cpu6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Modifies Z, N, and V flags with fetched value
uint8_t Cpu6502::BIT()
{
	fetch();

	uint16_t temp = a & fetched;

	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));

	return 0;
}

//Branch if negative bit is set
uint8_t Cpu6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Branch if zero bit is not set
uint8_t Cpu6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Branch if negative bit is not set
uint8_t Cpu6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//BREAK
uint8_t Cpu6502::BRK()
{
	pc++;

	SetFlag(I, 1);
	write(STACK_POINTER_BASE_LOCATION + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(STACK_POINTER_BASE_LOCATION + sp, pc & 0x00FF);
	sp--;

	SetFlag(B, 1);
	write(STACK_POINTER_BASE_LOCATION + sp, status);
	sp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(INTERRUPT_SERVICE_BASE_LOCATION) | ((uint16_t)read(MAX_ADDRESS) << 8);

	return 0;
}

//Branch if overflow bit is not set
uint8_t Cpu6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//Branch if overflow bit is set
uint8_t Cpu6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		//Add extra clock cycle if page boundary is crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

//CLEAR carry bit
uint8_t Cpu6502::CLC()
{
	SetFlag(C, false);
	return 0;
}

//CLEAR decimal mode bit
uint8_t Cpu6502::CLD()
{
	SetFlag(D, false);
	return 0;
}

//CLEAR interrupt bit
uint8_t Cpu6502::CLI()
{
	SetFlag(I, false);
	return 0;
}

//CLEAR overflow bit
uint8_t Cpu6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

//Compare accumulator and memory
uint8_t Cpu6502::CMP()
{
	fetch();

	uint16_t temp = (uint16_t)a - (uint16_t)fetched;

	SetFlag(C, a >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	return 1;
}

//Compare X register to memory
uint8_t Cpu6502::CPX()
{
	fetch();

	uint16_t temp = (uint16_t)x - (uint16_t)fetched;

	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	return 0;
}

//Compare Y register to memory
uint8_t Cpu6502::CPY()
{
	fetch();

	uint16_t temp = (uint16_t)y - (uint16_t)fetched;

	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	return 0;
}

//Decrement value at memory location
uint8_t Cpu6502::DEC()
{
	uint8_t temp = fetch();
	temp--;

	SetFlag(N, temp & 0x80);
	SetFlag(Z, temp == 0x00);

	write(addr_abs, temp);

	return 0;
}

//Decrement value in x register
uint8_t Cpu6502::DEX()
{
	x--;

	SetFlag(N, x & 0x80);
	SetFlag(Z, x == 0x00);

	return 0;
}

//Decrement value in y register
uint8_t Cpu6502::DEY()
{
	y--;

	SetFlag(N, y & 0x80);
	SetFlag(Z, y == 0x00);

	return 0;
}

//XOR a, m
uint8_t Cpu6502::EOR()
{
	uint8_t temp = fetch();

	a ^= temp;

	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 1;
}

//Increment value at memory location
uint8_t Cpu6502::INC()
{
	uint8_t temp = fetch();

	temp++;

	SetFlag(Z, temp == 0x00);
	SetFlag(N, temp & 0x80);

	write(addr_abs, temp);

	return 0;
}

//Increment x register
uint8_t Cpu6502::INX()
{
	x++;

	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);

	return 0;
}

//Increment y register
uint8_t Cpu6502::INY()
{
	y++;

	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);

	return 0;
}

//JUMP to location
uint8_t Cpu6502::JMP()
{
	pc = addr_abs;

	return 0;
}

//JUMP to subroutine
uint8_t Cpu6502::JSR()
{
	pc--;

	write(STACK_POINTER_BASE_LOCATION + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(STACK_POINTER_BASE_LOCATION + sp, pc & 0x00FF);
	sp--;

	pc = addr_abs;

	return 0;
}

//LD a, m
uint8_t Cpu6502::LDA()
{
	a = fetch();

	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 1;
}

//LD x, m
uint8_t Cpu6502::LDX()
{
	x = fetch();

	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);

	return 1;
}

//LD y, m
uint8_t Cpu6502::LDY()
{
	y = fetch();

	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);

	return 1;
}

//Shift right
uint8_t Cpu6502::LSR()
{
	fetch();

	SetFlag(C, fetched & 0x0001);

	uint16_t temp = fetched >> 1;

	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	if (lookup[opcode].addrmode == &Cpu6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);

	return 0;
}

//Handle illegal opcodes
uint8_t Cpu6502::NOP()
{
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}

	return 0;
}

//Bitwise XOR, a, m
uint8_t Cpu6502::ORA()
{
	fetch();

	a |= fetched;

	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 1;
}

//PUSH accumulator to stack
uint8_t Cpu6502::PHA()
{
	write(STACK_POINTER_BASE_LOCATION + sp, a);
	sp--;

	return 0;
}

//PUSH status register to stack
uint8_t Cpu6502::PHP()
{
	write(STACK_POINTER_BASE_LOCATION + sp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	sp--;

	return 0;
}

//POP accumulator from stack
uint8_t Cpu6502::PLA()
{
	sp++;
	a = read(STACK_POINTER_BASE_LOCATION + sp);
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 0;
}

//POP status register from stack
uint8_t Cpu6502::PLP()
{
	sp++;
	status = read(STACK_POINTER_BASE_LOCATION + sp);
	SetFlag(U, 1);

	return 0;
}

//Rotate left
uint8_t Cpu6502::ROL()
{
	fetch();

	uint16_t temp = (uint16_t)(fetched << 1) | GetFlag(C);

	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	if (lookup[opcode].addrmode == &Cpu6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);

	return 0;
}

//Rotate right
uint8_t Cpu6502::ROR()
{
	fetch();

	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);

	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);

	if (lookup[opcode].addrmode == &Cpu6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);

	return 0;
}

//Return from interrupt and restore previous state
uint8_t Cpu6502::RTI()
{
	sp++;
	status = read(STACK_POINTER_BASE_LOCATION + sp);
	status &= ~B;
	status &= ~U;

	sp++;
	pc = (uint16_t)read(STACK_POINTER_BASE_LOCATION + sp);
	sp++;
	pc |= (uint16_t)read(STACK_POINTER_BASE_LOCATION + sp) << 8;

	return 0;
}

//Return from subroutine
uint8_t Cpu6502::RTS()
{
	sp++;
	pc = (uint16_t)read(STACK_POINTER_BASE_LOCATION + sp);
	sp++;
	pc |= (uint16_t)read(STACK_POINTER_BASE_LOCATION + sp) << 8;

	pc++;

	return 0;
}

//SUBTRACT fetched from accumulator
uint8_t Cpu6502::SBC()
{
	fetch();

	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	uint16_t temp = (uint16_t)a + value + (uint16_t)GetFlag(C);

	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);

	a = temp & 0x00FF;

	return 1;
}

//Set carry flag
uint8_t Cpu6502::SEC()
{
	SetFlag(C, true);

	return 0;
}

//Set decimal flag
uint8_t Cpu6502::SED()
{
	SetFlag(D, true);

	return 0;
}

//Set interrupt flag
uint8_t Cpu6502::SEI()
{
	SetFlag(I, true);

	return 0;
}

//Store a, m
uint8_t Cpu6502::STA()
{
	write(addr_abs, a);

	return 0;
}

//Store x, m
uint8_t Cpu6502::STX()
{
	write(addr_abs, x);

	return 0;
}

//Store y, m
uint8_t Cpu6502::STY()
{
	write(addr_abs, y);

	return 0;
}

//Transfer a, x
uint8_t Cpu6502::TAX()
{
	x = a;

	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);

	return 0;
}

//Transfer a, y
uint8_t Cpu6502::TAY()
{
	y = a;

	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);

	return 0;
}

//Transfer sp, x
uint8_t Cpu6502::TSX()
{
	x = sp;

	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);

	return 0;
}

//Transfer x, a
uint8_t Cpu6502::TXA()
{
	a = x;

	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 0;
}

//Transfer x, sp
uint8_t Cpu6502::TXS()
{
	sp = x;

	return 0;
}

//Transfer y, a
uint8_t Cpu6502::TYA()
{
	a = y;

	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);

	return 0;
}

//Capture illegal opcodes
uint8_t Cpu6502::XXX()
{
	return 0;
}

#pragma endregion

//Helpers
#pragma region

bool Cpu6502::complete()
{
	return cycles == 0;
}

#pragma endregion