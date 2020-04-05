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
		TEST_METHOD(TestSUB)
		{
			cpu.registers.A = 10;
			cpu.registers.B = 2;
			cpu.registers.C = 10;

			cpu.Execute(EInstruction::SUB, ERegisterTarget::B);
			Assert::AreEqual(8, static_cast<int>(cpu.registers.A));
			Assert::IsFalse(cpu.registers.GetFlags().Carry);
			Assert::IsFalse(cpu.registers.GetFlags().HalfCarry);

			cpu.Execute(EInstruction::SUB, ERegisterTarget::C);
			Assert::AreEqual(254, static_cast<int>(cpu.registers.A));
			Assert::IsTrue(cpu.registers.GetFlags().Carry);
			Assert::IsTrue(cpu.registers.GetFlags().HalfCarry);
		}
		TEST_METHOD(TestSUBC)
		{
			cpu.registers.A = 8;
			cpu.registers.B = 10;

			cpu.Execute(EInstruction::SUBC, ERegisterTarget::B);
			Assert::AreEqual(253, static_cast<int>(cpu.registers.A));
			Assert::IsTrue(cpu.registers.GetFlags().Carry);
		}
		TEST_METHOD(TestAndOr)
		{
			cpu.registers.A = 200;
			cpu.registers.B = 125;

			cpu.Execute(EInstruction::AND, ERegisterTarget::B);
			Assert::AreEqual(72, static_cast<int>(cpu.registers.A));
			Assert::IsFalse(cpu.registers.GetFlags().Carry);
			Assert::IsFalse(cpu.registers.GetFlags().Subtraction);
			Assert::IsTrue(cpu.registers.GetFlags().HalfCarry);

			cpu.registers.A = 200;
			cpu.registers.B = 125;
			cpu.Execute(EInstruction::OR, ERegisterTarget::B);
			Assert::AreEqual(253, static_cast<int>(cpu.registers.A));
			Assert::IsFalse(cpu.registers.GetFlags().Carry);
			Assert::IsFalse(cpu.registers.GetFlags().Subtraction);
			Assert::IsFalse(cpu.registers.GetFlags().HalfCarry);
		}
	};
}
