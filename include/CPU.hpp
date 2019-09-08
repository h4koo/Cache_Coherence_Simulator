
#include <iostream>
#include <string>
#include "InstructionGenerator.hpp"
#include "Clock.hpp"
#include "CacheController.hpp"

#ifndef CPU_CORE
#define CPU_CORE

namespace cpucore
{

enum CPU_STATUS
{
    READY,
    WAITING,
    WORKING
};

class CPU
{
private:
    simulationcomputer::Clock clk;
    CacheController _cache;
    InstructionGenerator _inst_gen;
    Instruction _current_instruction;

    std::string _id;
    CPU_STATUS _status;

    std::string _data_in;
    std::string _data_out;

    int _total_cycles;
    int _waiting_cycles;

    // this function requests an instruction from the instruction generator
    bool fetchInstruction();

    // this function checks the type of instruction and performs the need actions
    bool executeInstruction();

    // this function requests to read memory from the cache controller
    void readMem();

    // this function requests to write memory from the cache controller
    void writeMem();

    //this function dumps processor information into a file
    bool dumpToFile();

public:
    CPU(std::string id, simulationcomputer::CpuPort *bus_port);

    //this function starts the CPU functioning, starts to count cycles and ticks the clock
    void loop();
};

} // namespace cpucore

#endif