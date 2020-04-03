#include <iostream>
#include <cassert>

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
	uint16_t GetBC() const {
		return static_cast<uint16_t>(B) << 8 | C;
	}

	void SetBC(uint16_t val) {
		B = (val >> 8) & 0xFF;
		C = val & 0xFF;
	}
};

enum class eInstruction {
	ADD
};
enum class eRegisterTarget {
	A, B, C, D, E, H, L
};

class Cpu {
public:
	SRegisters registers;
	void Execute(eInstruction instruction, eRegisterTarget registerTarget) {
		uint8_t value = 0;
		switch (instruction)
		{
			// Steps for ADD
			// Read the current value from the target register
			// Add the value to the value in register A making sure to handle overflow
			// Update register F(flags)
			// Write the updated value to register A
		case eInstruction::ADD:
			switch (registerTarget)
			{
			case eRegisterTarget::A:
				value = registers.A;
				break;
			case eRegisterTarget::B:
				value = registers.B;
				break;
			case eRegisterTarget::C:
				value = registers.C;				
				break;
			case eRegisterTarget::D:
				value = registers.D;
				break;
			case eRegisterTarget::E:
				value = registers.E;
				break;
			case eRegisterTarget::H:
				value = registers.H;
				break;
			case eRegisterTarget::L:
				value = registers.L;
				break;
			default:
				assert(false);
				break;
			}
			registers.A = add(value);
			break;
		default:
			break;
		}
	}

private:
	// Check if int overflew
	static bool didOverflow(uint8_t a, uint8_t b)
	{
		return (a + b > UINT8_MAX ? true : false);
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


};



int main() {
	Cpu cpu{};
	cpu.registers.A = 250;
	cpu.registers.C = 10;
	cpu.Execute(eInstruction::ADD, eRegisterTarget::C);

	return 0;
}