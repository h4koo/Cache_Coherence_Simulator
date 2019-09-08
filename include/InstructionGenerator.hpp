
#include <iostream>
#include <string>

#ifndef INST_GEN
#define INST_GEN

namespace cpucore
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

typedef struct
{
    /* data */
    InstructionType inst_type;
    int inst_address;
} Instruction;

class InstructionGenerator
{

    int mem_gen_percentage;
    ProbDistribution probability_distribution;

public:
    InstructionGenerator();

    Instruction generateInstruction();
};

} // namespace cpucore

#endif