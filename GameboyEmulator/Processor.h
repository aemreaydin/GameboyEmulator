#pragma once
#include <iostream>

// Gameboy CPU is an 8-bit CPU
// Has 8 registers
struct SRegisters {
	uint8_t A, B, C, D, E, H, L;
	// F will be the flags register, lower four bits will always be 0
	// upper four bits will correspond to certain states
	// Bit 7: Zero
	// Bit 6: Subtraction
	// Bit 5: Half Carry
	// Bit 4: Carry
	uint8_t F;
	struct SFlagRegister {
		bool Zero = false;
		bool Subtraction = false;
		bool HalfCarry = false;
		bool Carry = false;

		static const uint8_t ZERO_FLAG_POSITION = 7;
		static const uint8_t SUBTRACTION_FLAG_POSITION = 6;
		static const uint8_t HALF_CARRY_FLAG_POSITION = 5;
		static const uint8_t CARRY_FLAG_POSITION = 4;
	};

	SFlagRegister GetFlags() const {
		return SFlagRegister{
			(F >> SFlagRegister::ZERO_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::SUBTRACTION_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::HALF_CARRY_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::CARRY_FLAG_POSITION) & 0b1 ? true : false,
		};
	}

	void SetFlags(const SFlagRegister fReg) {
		F |= fReg.Zero << SFlagRegister::ZERO_FLAG_POSITION;
		F |= fReg.Subtraction << SFlagRegister::SUBTRACTION_FLAG_POSITION;
		F |= fReg.HalfCarry << SFlagRegister::HALF_CARRY_FLAG_POSITION;
		F |= fReg.Carry << SFlagRegister::CARRY_FLAG_POSITION;
	}

	// Somewhat of an hack that makes the registers 16-bit
	// For the instructions that allow 16-bit read/write
	uint16_t GetAF() const {
		return static_cast<uint16_t>(A) << 8 | F;
	}
	uint16_t GetBC() const {
		return static_cast<uint16_t>(B) << 8 | C;
	}
	uint16_t GetDE() const {
		return static_cast<uint16_t>(D) << 8 | E;
	}
	uint16_t GetHL() const {
		return static_cast<uint16_t>(H) << 8 | L;
	}

	void SetAF(const uint16_t val) {
		A = (val >> 8) & 0xFF;
		F = val & 0xFF;
	}
	void SetBC(const uint16_t val) {
		B = (val >> 8) & 0xFF;
		C = val & 0xFF;
	}
	void SetDE(const uint16_t val) {
		D = (val >> 8) & 0xFF;
		E = val & 0xFF;
	}
	void SetHL(const uint16_t val) {
		H = (val >> 8) & 0xFF;
		L = val & 0xFF;
	}
};

enum class EInstruction {
	ADD,
	ADDC,
	ADDHL
};
enum class ERegisterTarget {
	A, B, C, D, E, H, L, AF, BC, DE, HL
};

class CProcessor {
public:
	SRegisters registers{};
	void Execute(const EInstruction instruction, const ERegisterTarget registerTarget) {

		switch (instruction)
		{

		case EInstruction::ADD:
			doAdd(registerTarget);
			break;
		case EInstruction::ADDC:
			doAddC(registerTarget);
			break;
		case EInstruction::ADDHL:
			doAddHL(registerTarget);
			break;
		default:
			break;
		}
	}

private:
	// Check if int overflew
	static bool didOverflow(const uint8_t a, const uint8_t b)
	{
		return (a + b > UINT8_MAX ? true : false);
	}
	static bool didOverflow(const uint16_t a, const uint16_t b)
	{
		return (a + b > UINT16_MAX ? true : false);
	}

	// Steps for ADD
	// Read the current value from the target register
	// Add the value to the value in register A making sure to handle overflow
	// Update register F(flags)
	// Write the updated value to register A
	void doAdd(const ERegisterTarget registerTarget)
	{
		uint8_t value = 0;
		switch (registerTarget)
		{
		case ERegisterTarget::A:
			value = registers.A;
			break;
		case ERegisterTarget::B:
			value = registers.B;
			break;
		case ERegisterTarget::C:
			value = registers.C;
			break;
		case ERegisterTarget::D:
			value = registers.D;
			break;
		case ERegisterTarget::E:
			value = registers.E;
			break;
		case ERegisterTarget::H:
			value = registers.H;
			break;
		case ERegisterTarget::L:
			value = registers.L;
			break;
		default:
			break;
		}
		registers.A = add(value);
	}
	void doAddC(const ERegisterTarget registerTarget)
	{
		uint8_t value = 0;
		switch (registerTarget)
		{
		case ERegisterTarget::A:
			value = registers.A;
			break;
		case ERegisterTarget::B:
			value = registers.B;
			break;
		case ERegisterTarget::C:
			value = registers.C;
			break;
		case ERegisterTarget::D:
			value = registers.D;
			break;
		case ERegisterTarget::E:
			value = registers.E;
			break;
		case ERegisterTarget::H:
			value = registers.H;
			break;
		case ERegisterTarget::L:
			value = registers.L;
			break;
		default:
			break;
		}
		registers.A = addC(value);
	}
	// Will only be used in virtual 16-bit registers
	void doAddHL(const ERegisterTarget registerTarget)
	{
		uint16_t value = 0;
		switch (registerTarget)
		{
		case ERegisterTarget::AF:
			value = registers.GetAF();
			break;
		case ERegisterTarget::BC:
			value = registers.GetBC();
			break;
		case ERegisterTarget::DE:
			value = registers.GetDE();
			break;
		case ERegisterTarget::HL:
			value = registers.GetHL();
			break;
		default:
			break;
		}
		registers.SetHL(addHL(value));
	}


	uint8_t add(const uint8_t value) {
		const uint8_t newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = didOverflow(registers.A, value);
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
		registers.SetFlags(fReg);

		return newValue;
	}
	// Half carry set if carry from bit 11
	// Carry set if carry from bit 15
	uint16_t addHL(const uint16_t value) {
		const uint16_t newValue = registers.GetHL() + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = didOverflow(registers.GetHL(), value);
		fReg.HalfCarry = (registers.GetHL() & 0xFFF) + (value & 0xFFF) > 0xFFF;
		registers.SetFlags(fReg);

		return newValue;
	}
	// Also add the carry flag
	uint8_t addC(const uint8_t value) {
		const uint8_t newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = didOverflow(registers.A, value);
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
		registers.SetFlags(fReg);

		return fReg.Carry ? newValue + 0b1 : newValue;
	}
};

