#include "pch.h"
#include "CppUnitTest.h"

#include "../GameboyEmulator/Processor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GbEmulatorTest
{
	
	TEST_CLASS(GbEmulatorTest)
	{
	public:
		CProcessor Cpu;

		TEST_METHOD(TestAdd)
		{
			Cpu.Registers.A = 250;
			Cpu.Registers.B = 5;
			Cpu.Registers.C = 5;

			Cpu.Execute(EInstruction::ADD, ERegisterTarget::B);
			Assert::AreEqual(255, static_cast<int>(Cpu.Registers.A));
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);

			Cpu.Execute(EInstruction::ADD, ERegisterTarget::C);
			Assert::AreEqual(4, static_cast<int>(Cpu.Registers.A));
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);
		}
		TEST_METHOD(TestAddC)
		{
			Cpu.Registers.A = 250;
			Cpu.Registers.B = 10;

			Cpu.Execute(EInstruction::ADDC, ERegisterTarget::B);
			Assert::AreEqual(5, static_cast<int>(Cpu.Registers.A));
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);
		}
		TEST_METHOD(TestAddHL)
		{
			Cpu.Registers.SetHL(65530);
			Cpu.Registers.SetBC(10);

			Cpu.Execute(EInstruction::ADDHL, ERegisterTarget::BC);
			Assert::AreEqual(4, static_cast<int>(Cpu.Registers.GetHL()));
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);

			Cpu.Registers.SetHL(4095);
			Cpu.Execute(EInstruction::ADDHL, ERegisterTarget::BC);
			Assert::AreEqual(4105, static_cast<int>(Cpu.Registers.GetHL()));
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);
		}
		TEST_METHOD(TestSub)
		{
			Cpu.Registers.A = 10;
			Cpu.Registers.B = 2;
			Cpu.Registers.C = 10;

			Cpu.Execute(EInstruction::SUB, ERegisterTarget::B);
			Assert::AreEqual(8, static_cast<int>(Cpu.Registers.A));
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);
			Assert::IsFalse(Cpu.Registers.GetFlags().HalfCarry);

			Cpu.Execute(EInstruction::SUB, ERegisterTarget::C);
			Assert::AreEqual(254, static_cast<int>(Cpu.Registers.A));
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);
		}
		TEST_METHOD(TestSubC)
		{
			Cpu.Registers.A = 8;
			Cpu.Registers.B = 10;

			Cpu.Execute(EInstruction::SUBC, ERegisterTarget::B);
			Assert::AreEqual(253, static_cast<int>(Cpu.Registers.A));
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);
		}
		TEST_METHOD(TestAndOrXor)
		{
			Cpu.Registers.A = 200;
			Cpu.Registers.B = 125;
			
			Cpu.Execute(EInstruction::AND, ERegisterTarget::B);
			Assert::AreEqual(72, static_cast<int>(Cpu.Registers.A));
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);
			Assert::IsFalse(Cpu.Registers.GetFlags().Subtraction);
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);

			
			Cpu.Registers.A = 200;
			Cpu.Registers.B = 125;
			
			Cpu.Execute(EInstruction::OR, ERegisterTarget::B);
			Assert::AreEqual(253, static_cast<int>(Cpu.Registers.A));
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);
			Assert::IsFalse(Cpu.Registers.GetFlags().Subtraction);
			Assert::IsFalse(Cpu.Registers.GetFlags().HalfCarry);

			Cpu.Registers.A = 200;
			Cpu.Registers.B = 125;

			Cpu.Execute(EInstruction::XOR, ERegisterTarget::B);
			Assert::AreEqual(181, static_cast<int>(Cpu.Registers.A));
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);
			Assert::IsFalse(Cpu.Registers.GetFlags().Subtraction);
			Assert::IsFalse(Cpu.Registers.GetFlags().HalfCarry);			
		}
		TEST_METHOD(TestCp)
		{
			Cpu.Registers.A = 200;
			Cpu.Registers.B = 200;

			Cpu.Execute(EInstruction::CP, ERegisterTarget::B);
			Assert::AreEqual(200, static_cast<int>(Cpu.Registers.A));
			Assert::IsTrue(Cpu.GetRegisters().GetFlags().Zero);
			Assert::IsTrue(Cpu.Registers.GetFlags().Subtraction);
			Assert::IsFalse(Cpu.Registers.GetFlags().HalfCarry);
			Assert::IsFalse(Cpu.Registers.GetFlags().Carry);

			Cpu.Registers.A = 8;
			Cpu.Registers.B = 10;
			Cpu.Execute(EInstruction::CP, ERegisterTarget::B);
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);
			Assert::IsTrue(Cpu.Registers.GetFlags().Carry);
		}

		TEST_METHOD(TestInc)
		{
			Cpu.Registers.B = 200;

			Cpu.Execute(EInstruction::INC, ERegisterTarget::B);
			Assert::AreEqual(201, static_cast<int>(Cpu.Registers.B));
			Assert::IsFalse(Cpu.GetRegisters().GetFlags().Zero);
			Assert::IsFalse(Cpu.Registers.GetFlags().Subtraction);
			Assert::IsFalse(Cpu.Registers.GetFlags().HalfCarry);

			Cpu.Registers.B = 15;
			Cpu.Execute(EInstruction::INC, ERegisterTarget::B);
			Assert::AreEqual(16, static_cast<int>(Cpu.Registers.B));
			Assert::IsTrue(Cpu.Registers.GetFlags().HalfCarry);
		}
	};
}
