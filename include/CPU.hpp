
#include <iostream>
#include <string>
#include <future>
#include <thread>
#include "InstructionGenerator.hpp"
#include "Clock.hpp"
#include "CacheController.hpp"

#ifndef CPU_CORE
#define CPU_CORE

namespace cpucore
{
const int CPU_BASE_CLOCK = 1;

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

    std::string _read_data;
    int _total_cycles;
    int _waiting_cycles;

    // this function requests an instruction from the instruction generator
    void fetchInstruction();

    // this function checks the type of instruction and performs the need actions
    void executeInstruction();

    // this function requests to read memory from the cache controller
    void readMem();

    // this function requests to write memory from the cache controller
    void writeMem();

    //this function dumps processor information into a file
    void dumpToFile();

public:
    CPU(std::string id) : clk(CPU_BASE_CLOCK), _id(id), _status(READY), _total_cycles(0), _waiting_cycles(0)
    {
        printf("Created CPU id: %s\n", id.c_str());
    };

    CPU(std::string id, simulationcomputer::CpuPort *bus_port) : clk(CPU_BASE_CLOCK), _cache(bus_port), _id(id), _status(READY){};

    void connectBusPort(simulationcomputer::CpuPort *bus_port) { _cache.connectBusPort(bus_port); };

    CacheController *getCacheController()
    {
        return &_cache;
    }

    //this function starts the CPU functioning, starts to count cycles and ticks the clock
    void loop();

    void work();

    void tick();
};

} // namespace cpucore

#endif