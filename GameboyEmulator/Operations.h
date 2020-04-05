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
		fReg.Carry = DidOverflow(registers.A, value);
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
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
		fReg.Carry = DidOverflow(registers.GetHL(), value);
		fReg.HalfCarry = (registers.GetHL() & 0xFFF) + (value & 0xFFF) > 0xFFF;
		registers.SetFlags(fReg);

		registers.SetHL(newValue);
	}
	// Also add the carry flag
	static void AddC(const U8 value, SRegisters& registers) {
		const U8 newValue = registers.A + value;
		SRegisters::SFlagRegister fReg;
		fReg.Zero = newValue == 0;
		fReg.Subtraction = false;
		fReg.Carry = DidOverflow(registers.A, value);
		fReg.HalfCarry = (registers.A & 0xF) + (value & 0xF) > 0xF;
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
		fReg.Carry = registers.A - value < 0;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
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
		fReg.Carry = registers.A - value < 0;
		fReg.HalfCarry = (registers.A & 0xF) - (value & 0xF) < 0;
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
		fReg.Carry = false;
		fReg.HalfCarry = true;
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
		fReg.Carry = false;
		fReg.HalfCarry = false;
		registers.SetFlags(fReg);

		registers.A = newValue;
	}
};
