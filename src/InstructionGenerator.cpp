
#include "../include/InstructionGenerator.hpp"

namespace cpucore{
    InstructionGenerator::InstructionGenerator()
    {
    }

    Instruction InstructionGenerator::generateInstruction()
    {
        srand(time(NULL));
        InstructionType types[3] = {WORK, READ_MEM, WRITE_MEM};

        Instruction ret;

        ret.inst_type = types[rand() % 3];
        ret.inst_address = rand() % 8;

        return ret;
    }
}// namespace CPU_CORE