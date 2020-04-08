#pragma once
#include <iostream>

using U8 = uint8_t;
using U16 = uint16_t;

enum class EInstruction
{
	ADD,
	ADDC,
	ADDHL,
	SUB,
	SUBC,
	AND,
	OR,
	XOR,
	CP,
	INC,
	DEC,
	INC16,
	DEC16,
	CCF,
	SCF,
	RRA,
	RLA,
	RRCA,
	RRLA,
	CPL,
	BIT,
	RESET,
	SET,
	SRL,
	RR,
	RL,
	RRC,
	RLC,
	SRA,
	SLA,
	SWAP
};

enum class ERegisterTarget
{
	A,
	B,
	C,
	D,
	E,
	H,
	L,
	AF,
	BC,
	DE,
	HL,
	UNK
};

struct SRegisters
{
private:
	U8 F{};
public:
	U8 A{}, B{}, C{}, D{}, E{}, H{}, L{};

	// F will be the flags register, lower four bits will always be 0
	// upper four bits will correspond to certain states
	// Bit 7: Zero
	// Bit 6: Subtraction
	// Bit 5: Half Carry
	// Bit 4: Carry
	struct SFlagRegister
	{
		bool Zero = false;
		bool Subtraction = false;
		bool HalfCarry = false;
		bool Carry = false;

		static const U8 ZERO_FLAG_POSITION = 7;
		static const U8 SUBTRACTION_FLAG_POSITION = 6;
		static const U8 HALF_CARRY_FLAG_POSITION = 5;
		static const U8 CARRY_FLAG_POSITION = 4;
	};

	[[nodiscard]] SFlagRegister GetFlags() const
	{
		return SFlagRegister{
			(F >> SFlagRegister::ZERO_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::SUBTRACTION_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::HALF_CARRY_FLAG_POSITION) & 0b1 ? true : false,
			(F >> SFlagRegister::CARRY_FLAG_POSITION) & 0b1 ? true : false,
		};
	}

	void SetFlags(const SFlagRegister fReg)
	{
		F = 0;
		F |= fReg.Zero << SFlagRegister::ZERO_FLAG_POSITION;
		F |= fReg.Subtraction << SFlagRegister::SUBTRACTION_FLAG_POSITION;
		F |= fReg.HalfCarry << SFlagRegister::HALF_CARRY_FLAG_POSITION;
		F |= fReg.Carry << SFlagRegister::CARRY_FLAG_POSITION;
	}

	// Somewhat of an hack that makes the Registers 16-bit
	// For the instructions that allow 16-bit read/write
	[[nodiscard]] U16 GetAF() const
	{
		return static_cast<U16>(A) << 8 | F;
	}

	[[nodiscard]] U16 GetBC() const
	{
		return static_cast<U16>(B) << 8 | C;
	}

	[[nodiscard]] U16 GetDE() const
	{
		return static_cast<U16>(D) << 8 | E;
	}

	[[nodiscard]] U16 GetHL() const
	{
		return static_cast<U16>(H) << 8 | L;
	}

	void SetAF(const U16 val)
	{
		A = (val >> 8) & 0xFF;
		F = val & 0xFF;
	}

	void SetBC(const U16 val)
	{
		B = (val >> 8) & 0xFF;
		C = val & 0xFF;
	}

	void SetDE(const U16 val)
	{
		D = (val >> 8) & 0xFF;
		E = val & 0xFF;
	}

	void SetHL(const U16 val)
	{
		H = (val >> 8) & 0xFF;
		L = val & 0xFF;
	}
};

// Check if int overflew
static bool DidOverflow(const U8 a, const U8 b)
{
	return (a + b > UINT8_MAX ? true : false);
}

static bool DidOverflow(const U16 a, const U16 b)
{
	return (a + b > UINT16_MAX ? true : false);
}
