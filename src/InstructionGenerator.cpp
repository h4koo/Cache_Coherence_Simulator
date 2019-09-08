
#include <iostream>
#include <string>

namespace CPU_CORE
{

enum InstructionType
{
    WORK,
    READ_MEM,
    WRITE_MEM
};

enum ProbDistribution
{
    CUADRATIC,
    BINOMIAL
};

struct Instruction
{
    /* data */
    InstructionType inst_type;
    int inst_address;
};

class InstructionGenerator
{

    int mem_gen_percentage;
    ProbDistribution probability_distribution;

public:
    InstructionGenerator()
    {
    }

    Instruction generateInstruction()
    {
        Instruction ret;

        return ret;
    }
};

} // namespace CPU_CORE