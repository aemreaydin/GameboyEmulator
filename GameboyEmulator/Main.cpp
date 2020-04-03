#include <iostream>
#include <bitset>

// Gameboy CPU is an 8-bit CPU
// Has 8 registers
struct sRegisters {
	uint8_t a, b, c, d, e, h, l;
	// f will be the flags register, lower four bits will always be 0
	// upper four bits will correspond to certain states
	// Bit 7: Zero
	// Bit 6: Subtraction
	// Bit 5: Half Carry
	// Bit 4: Carry
	uint8_t f;
	struct sFlagRegister {
		bool zero;
		bool subtraction;
		bool halfCarry;
		bool carry;

		static const uint8_t ZERO_FLAG_POSITION = 7;
		static const uint8_t SUBTRACTION_FLAG_POSITION = 6;
		static const uint8_t HALF_CARRY_FLAG_POSITION = 5;
		static const uint8_t CARRY_FLAG_POSITION = 4;
	};

	sFlagRegister GetFlags() {
		return sFlagRegister{
			(f >> sFlagRegister::ZERO_FLAG_POSITION) & 0b1 ? true : false,
			(f >> sFlagRegister::SUBTRACTION_FLAG_POSITION) & 0b1 ? true : false,
			(f >> sFlagRegister::HALF_CARRY_FLAG_POSITION) & 0b1 ? true : false,
			(f >> sFlagRegister::CARRY_FLAG_POSITION) & 0b1 ? true : false,
		};
	}

	void SetFlags(sFlagRegister fReg) {
		f |= fReg.zero << sFlagRegister::ZERO_FLAG_POSITION;
		f |= fReg.subtraction << sFlagRegister::SUBTRACTION_FLAG_POSITION;
		f |= fReg.halfCarry << sFlagRegister::HALF_CARRY_FLAG_POSITION;
		f |= fReg.carry << sFlagRegister::CARRY_FLAG_POSITION;
	}

	// Somewhat of an hack that makes the registers 16-bit
	// For the instructions that allow 16-bit read/write
	uint16_t GetBC() {
		return static_cast<uint16_t>(b) << 8 | c;
	}

	void SetBC(uint16_t val) {
		b = (val >> 8) & 0xFF;
		c = val & 0xFF;
	}
};


int main() {

	uint8_t a = 255;
	uint8_t b = 200;
	uint16_t c = (a << 8) | b;

	sRegisters reg;
	reg.b = 255;
	reg.c = 200;
	std::cout << std::bitset<8>(b) << std::endl;
	auto st = reg.GetBC();
	uint8_t ab = (st >> 8) & 0xFF;
	uint8_t bc = st & 0xFF;

	std::cout << std::bitset<1>(bc >> 7);
	std::cout << std::bitset<1>(bc >> 6);
	std::cout << std::bitset<1>(bc >> 5);
	std::cout << std::bitset<1>(bc >> 4);
	std::cout << std::bitset<1>(bc >> 3);
	std::cout << std::bitset<1>(bc >> 2);

	return 0;
}