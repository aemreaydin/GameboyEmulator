#include <iostream>
#include "Processor.h"

int main() {
	CProcessor cpu{};
	cpu.registers.A = 250;
	cpu.registers.B = 10;
	cpu.Execute(EInstruction::ADDC, ERegisterTarget::B);

	return 0;
}
