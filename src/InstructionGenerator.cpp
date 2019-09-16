
#include "../include/InstructionGenerator.hpp"

namespace cpucore
{
InstructionGenerator::InstructionGenerator()
{
    srand(time(NULL));
}

Instruction InstructionGenerator::generateInstruction()
{

    InstructionType types[3] = {WORK, READ_MEM, WRITE_MEM};

    Instruction ret;
    ret.inst_type = types[rand() % 3];

    ret.inst_address = rand() % 16;

    return ret;
}
} // namespace cpucore