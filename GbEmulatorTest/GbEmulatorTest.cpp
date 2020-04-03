#include "pch.h"
#include "CppUnitTest.h"

#include "../GameboyEmulator/Processor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GbEmulatorTest
{
	
	TEST_CLASS(GbEmulatorTest)
	{
	public:
		CProcessor cpu;
		TEST_METHOD(TestADD)
		{
			cpu.registers.A = 250;
			cpu.registers.B = 5;
			cpu.registers.C = 5;

			cpu.Execute(EInstruction::ADD, ERegisterTarget::B);
			Assert::AreEqual(255, static_cast<int>(cpu.registers.A));
			Assert::IsFalse(cpu.registers.GetFlags().Carry);

			cpu.Execute(EInstruction::ADD, ERegisterTarget::C);
			Assert::AreEqual(4, static_cast<int>(cpu.registers.A));
			Assert::IsTrue(cpu.registers.GetFlags().Carry);
			Assert::IsTrue(cpu.registers.GetFlags().HalfCarry);
		}
		TEST_METHOD(TestADDC)
		{
			cpu.registers.A = 250;
			cpu.registers.B = 10;

			cpu.Execute(EInstruction::ADDC, ERegisterTarget::B);
			Assert::AreEqual(5, static_cast<int>(cpu.registers.A));
			Assert::IsTrue(cpu.registers.GetFlags().Carry);
		}
		TEST_METHOD(TestADDHL)
		{
			cpu.registers.SetHL(65530);
			cpu.registers.SetBC(10);

			cpu.Execute(EInstruction::ADDHL, ERegisterTarget::BC);
			Assert::AreEqual(4, static_cast<int>(cpu.registers.GetHL()));
			Assert::IsTrue(cpu.registers.GetFlags().Carry);

			cpu.registers.SetHL(4095);
			cpu.Execute(EInstruction::ADDHL, ERegisterTarget::BC);
			Assert::AreEqual(4105, static_cast<int>(cpu.registers.GetHL()));
			Assert::IsTrue(cpu.registers.GetFlags().HalfCarry);
		}
	};
}
