#pragma once
#include <iostream>

using u8 = uint8_t;
using u16 = uint16_t;

// Gameboy CPU is an 8-bit CPU
// Has 8 registers
struct SRegisters {
private:
	u8 F{};
public:
	u8 A{}, B{}, C{}, D{}, E{}, H{}, L{};
	// F will be the flags register, lower four bits will always be 0
	// upper four bits will correspond to certain states
	// Bit 7: Zero
	// Bit 6: Subtraction
	// Bit 5: Half Carry
	// Bit 4: Carry
	struct SFlagRegister {
		bool Zero = false;
		bool Subtraction = false;
		bool HalfCarry = false;
		bool Carry = false;

		static const u8 ZERO_FLAG_POSITION = 7;
		static const u8 SUBTRACTION_FLAG_POSITION = 6;
		static const u8 HALF_CARRY_FLAG_POSITION = 5;
		static const u8 CARRY_FLAG_POSITION = 4;
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
	u16 GetAF() const {
		return static_cast<u16>(A) << 8 | F;
	}
	u16 GetBC() const {
		return static_cast<u16>(B) << 8 | C;
	}
	u16 GetDE() const {
		return static_cast<u16>(D) << 8 | E;
	}
	u16 GetHL() const {
		return static_cast<u16>(H) << 8 | L;
	}

	void SetAF(const u16 val) {
		A = (val >> 8) & 0xFF;
		F = val & 0xFF;
	}
	void SetBC(const u16 val) {
		B = (val >> 8) & 0xFF;
		C = val & 0xFF;
	}
	void SetDE(const u16 val) {
		D = (val >> 8) & 0xFF;
		E = val & 0xFF;
	}
	void SetHL(const u16 val) {
		H = (val >> 8) & 0xFF;
		L = val & 0xFF;
	}
};

enum class EInstruction {
	ADD,
	ADDC,
	ADDHL,
	SUB
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
		case EInstruction::SUB:
			doSub(registerTarget);
			break;
		default:
			break;
		}
	}

private:
	// Check if int overflew
	static bool didOverflow(const u8 a, const u8 b)
	{
		return (a + b > UINT8_MAX ? true : false);
	}
	static bool didOverflow(const u16 a, const u16 b)
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
		u8 value = 0;
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
		u8 value = 0;
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
		u16 value = 0;
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
	void doSub(const ERegisterTarget registerTarget)
	{
		u8 value = 0;
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
		registers.A = sub(value);
	}


	u8 add(const u8 value) {
		const u8 newValue = registers.A + value;
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
	u16 addHL(const u16 value) {
		const u16 newValue = registers.GetHL() + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = didOverflow(registers.GetHL(), value);
		fReg.HalfCarry = (registers.GetHL() & 0xFFF) + (value & 0xFFF) > 0xFFF;
		registers.SetFlags(fReg);

		return newValue;
	}
	// Also add the carry flag
	u8 addC(const u8 value) {
		const u8 newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = didOverflow(registers.A, value);
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
		registers.SetFlags(fReg);

		return fReg.Carry ? newValue + 0b1 : newValue;
	}
	// Sub instruction
	u8 sub(const u8 value)
	{
		const u8 newValue = registers.A - value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = true;
		fReg.Carry = registers.A - value < 0;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
		registers.SetFlags(fReg);

		return newValue;
	}
};

