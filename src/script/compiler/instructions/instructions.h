#pragma once

namespace script
{
	namespace compiler
	{
		class Instruction
		{
			size_t id;

		  public:
			~Instruction()
			{
			}

			void set_id(size_t i)
			{
				id = i;
			}
		};

		class LoadInstruction : public Instruction
		{
			size_t offset;
		};

		class AddInstruction : public Instruction
		{
		};
		class SubInstruction : public Instruction
		{
		};
		class MulInstruction : public Instruction
		{
		};
		class DivInstruction : public Instruction
		{
		};
		class ModInstruction : public Instruction
		{
		};

		class LabelInstruction : public Instruction
		{
		};

		class JumpInstruction : public Instruction
		{
			std::weak_ptr<LabelInstruction> dest;
		};

		class CallInstruction : public Instruction
		{
		  public:
			CallInstruction()
			{
			}
		};
	};
};