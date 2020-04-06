#pragma once
#include "Helpers.h"

class COperations
{
public:
	// Steps for ADD
	// Read the current value from the target register
	// Add the value to the value in register A making sure to handle overflow
	// Update register F(flags)
	// Write the updated value to register A
	static void Add(const U8 value, SRegisters& registers) {
		const U8 newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
		fReg.Carry = DidOverflow(registers.A, value);
		registers.SetFlags(fReg);

		registers.A = newValue;
	}
	// Half carry set if carry from bit 11
	// Carry set if carry from bit 15
	static void AddHL(const U16 value, SRegisters& registers) {
		const U16 newValue = registers.GetHL() + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = (registers.GetHL() & 0xFFF) + (value & 0xFFF) > 0xFFF;
		fReg.Carry = DidOverflow(registers.GetHL(), value);
		registers.SetFlags(fReg);

		registers.SetHL(newValue);
	}
	// Also add the carry flag
	static void AddC(const U8 value, SRegisters& registers) {
		const U8 newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
		fReg.Carry = DidOverflow(registers.A, value);
		registers.SetFlags(fReg);

		registers.A =  fReg.Carry ? newValue + 0b1 : newValue;
	}
	// Sub instruction
	static void Sub(const U8 value, SRegisters& registers)
	{
		const U8 newValue = registers.A - value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = true;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
		fReg.Carry = registers.A - value < 0;
		registers.SetFlags(fReg);

		registers.A = newValue;
	}
	// Sub with carry
	static void SubC(const U8 value, SRegisters& registers)
	{
		const U8 newValue = registers.A - value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = true;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
		fReg.Carry = registers.A - value < 0;
		registers.SetFlags(fReg);

		registers.A =  fReg.Carry ? newValue - 0b1 : newValue;
	}
	// N, C flags are reset, H is set
	static void AndOp(const U8 value, SRegisters& registers)
	{
		const U8 newValue = registers.A & value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = true;
		fReg.Carry = false;
		registers.SetFlags(fReg);

		registers.A = newValue;
	}
	// N, H, C flags are reset
	static void OrOp(const U8 value, SRegisters& registers)
	{
		const U8 newValue = registers.A | value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = false;
		fReg.Carry = false;
		registers.SetFlags(fReg);

		registers.A = newValue;
	}

	static void XorOp(const U8 value, SRegisters& registers)
	{
		const U8 newValue = registers.A ^ value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = false;
		fReg.Carry = false;
		registers.SetFlags(fReg);

		registers.A = newValue;
	}

	static void Cp(const U8 value, SRegisters& registers)
	{
		SRegisters::SFlagRegister fReg;
		fReg.Zero = registers.A == value;
		fReg.Subtraction = true;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
		fReg.Carry = registers.A - value < 0;
		registers.SetFlags(fReg);
	}

	static void Inc(const U8 value, SRegisters& registers, const ERegisterTarget registerTarget)
	{
		const auto newValue = value + 1;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.HalfCarry = (value & 0xF) + (1 & 0xF) > 0xF;
		fReg.Carry = registers.GetFlags().Carry;
		registers.SetFlags(fReg);

		switch (registerTarget)
		{
		case ERegisterTarget::A:
			registers.A = newValue;
			break;
		case ERegisterTarget::B:
			registers.B = newValue;
			break;
		case ERegisterTarget::C:
			registers.C = newValue;
			break;
		case ERegisterTarget::D:
			registers.D = newValue;
			break;
		case ERegisterTarget::E:
			registers.E = newValue;
			break;
		case ERegisterTarget::H:
			registers.H = newValue;
			break;
		case ERegisterTarget::L:
			registers.L = newValue;
			break;
		default:
			break;
		}
	}
	
	static void Dec(const U8 value, SRegisters& registers, const ERegisterTarget registerTarget)
	{
		const auto newValue = value - 1;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = true;
		fReg.HalfCarry = (value & 0xF) - (1 & 0xF) < 0;
		fReg.Carry = registers.GetFlags().Carry;
		registers.SetFlags(fReg);

		switch (registerTarget)
		{
		case ERegisterTarget::A:
			registers.A = newValue;
			break;
		case ERegisterTarget::B:
			registers.B = newValue;
			break;
		case ERegisterTarget::C:
			registers.C = newValue;
			break;
		case ERegisterTarget::D:
			registers.D = newValue;
			break;
		case ERegisterTarget::E:
			registers.E = newValue;
			break;
		case ERegisterTarget::H:
			registers.H = newValue;
			break;
		case ERegisterTarget::L:
			registers.L = newValue;
			break;
		default:
			break;
		}
	}
};
