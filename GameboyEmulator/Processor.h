#pragma once
#include "Helpers.h"
#include "Operations.h"


// Gameboy CPU is an 8-bit CPU
// Has 8 Registers
class CProcessor {
public:
	SRegisters Registers{};
	void Execute(const EInstruction instruction, const ERegisterTarget registerTarget) {
		switch (instruction)
		{
		case EInstruction::ADDHL:
			do16BitOps(instruction, registerTarget);
			break;
		case EInstruction::ADD: 
		case EInstruction::ADDC: 
		case EInstruction::SUB: 
		case EInstruction::SUBC: 
		case EInstruction::AND: 
		case EInstruction::OR:
			do8BitOps(instruction, registerTarget);
			break;
		default:
			break;
		}

	}

	SRegisters GetRegisters() const
	{
		return Registers;
	}
private:
	void do8BitOps(const EInstruction instruction, const ERegisterTarget registerTarget)
	{
		const auto value = getRegisterValueU8(registerTarget);
		switch (instruction)
		{
		case EInstruction::ADD:
			COperations::Add(value, Registers);
			break;
		case EInstruction::ADDC:
			COperations::AddC(value, Registers);
			break;
		case EInstruction::SUB:
			COperations::Sub(value, Registers);
			break;
		case EInstruction::SUBC:
			COperations::SubC(value, Registers);
			break;
		case EInstruction::AND:
			COperations::AndOp(value, Registers);
			break;
		case EInstruction::OR:
			COperations::OrOp(value, Registers);
			break;
		default:
			break;
		}
	}
	void do16BitOps(const EInstruction instruction, const ERegisterTarget registerTarget)
	{
		const auto value = getRegisterValueU16(registerTarget);
		switch (instruction)
		{
		case EInstruction::ADDHL:
			COperations::AddHL(value, Registers);
			break;
		default:
			break;
		}
	}
	U8 getRegisterValueU8(const ERegisterTarget registerTarget) const
	{
		U8 value = 0;
		switch (registerTarget)
		{
		case ERegisterTarget::A:
			value = Registers.A;
			break;
		case ERegisterTarget::B:
			value = Registers.B;
			break;
		case ERegisterTarget::C:
			value = Registers.C;
			break;
		case ERegisterTarget::D:
			value = Registers.D;
			break;
		case ERegisterTarget::E:
			value = Registers.E;
			break;
		case ERegisterTarget::H:
			value = Registers.H;
			break;
		case ERegisterTarget::L:
			value = Registers.L;
			break;
		default:
			break;
		}
		return value;
	}
	U16 getRegisterValueU16(const ERegisterTarget registerTarget) const
	{
		U16 value = 0;
		switch (registerTarget)
		{
		case ERegisterTarget::AF:
			value = Registers.GetAF();
			break;
		case ERegisterTarget::BC:
			value = Registers.GetBC();
			break;
		case ERegisterTarget::DE:
			value = Registers.GetDE();
			break;
		case ERegisterTarget::HL:
			value = Registers.GetHL();
			break;
		default:
			break;
		}
		return value;
	}
};

