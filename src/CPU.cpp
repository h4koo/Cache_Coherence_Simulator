
#include <iostream>
#include <string>
#include "InstructionGenerator.cpp"

namespace CPU_CORE
{

enum CPU_STATUS{
    READY,
    WAITING,
    WORKING
};

class CPU
{
private:
    std::string _status;
    std::string _instruction;
    std::string _cycles;

public:
    CPU()
    {
    }
};



} // namespace CPU_CORE