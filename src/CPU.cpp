#include "../include/CPU.hpp"

namespace cpucore
{
// this function requests an instruction from the instruction generator
void CPU::fetchInstruction()
{
    _current_instruction = _inst_gen.generateInstruction();
}

// this function checks the type of instruction and performs the needed actions
void CPU::executeInstruction()
{
    switch (_current_instruction.inst_type)
    {
    case WORK:
        _status = WORKING;
        dumpToFile();
        _status = READY;
        break;

    case READ_MEM:
    {
        _status = WAITING;

        //called as an asynchronous job
        std::thread cache_work(&CPU::readMem, this);
        cache_work.detach();
        // std::async(std::launch::async, &CPU::readMem, this);

        dumpToFile();

        break;
    }
    case WRITE_MEM:
{
        //called as an asynchronous job
        // std::async(std::launch::async, &CPU::writeMem, this);
        std::thread cache_work(&CPU::readMem, this);
        cache_work.detach();
        dumpToFile();
}

        break;
    }
}

// this function requests to read memory from the cache controller
void CPU::readMem()
{
    _cache.cpuRead(_current_instruction.inst_address);
}

// this function requests to write memory from the cache controller
void CPU::writeMem()
{
    _cache.cpuWrite(_current_instruction.inst_address, _id);
}

//this function dumps processor information into a file
void CPU::dumpToFile()
{

    printf("The processor with ID: %s has run for %d cycles", _id.c_str(), _total_cycles);
}

// CPU::CPU(std::string id) : _id(id), clk(CPU_BASE_CLOCK), _total_cycles(0), _waiting_cycles(0)
// {
// }

// CPU::CPU(std::string id, simulationcomputer::CpuPort *bus_port) : clk(CPU_BASE_CLOCK), _cache(bus_port)
// {
//     _id = id;
//     _total_cycles = 0;
//     _waiting_cycles = 0;
// }

//this function starts the CPU functioning, starts to count cycles and ticks the clock
void CPU::loop()
{

    while (true)
    {
        ++_total_cycles;
        switch (_status)
        {
        case READY:
            fetchInstruction();
            executeInstruction();

            break;
        case WAITING:
            if (_cache.getStatus() == RDY)
            { //the data we asked for is ready
                _read_data = _cache.getCpuDataLine();

                //possibly change state to request received to know what to print
                dumpToFile();

                //reset waiting cycle count
                _waiting_cycles = 0;
                _status = READY;
            }
            else
            { // still waiting
                ++_waiting_cycles;
                dumpToFile();
            }
            break;

        case WORKING:
            //should never get here
            break;
        }

        clk.tick();
    }
}

void CPU::work()
{
    ++_total_cycles;
    switch (_status)
    {
    case READY:
        fetchInstruction();
        executeInstruction();

        break;
    case WAITING:
        if (_cache.getStatus() == RDY)
        { //the data we asked for is ready
            _read_data = _cache.getCpuDataLine();

            //possibly change state to request received to know what to print
            dumpToFile();

            //reset waiting cycle count
            _waiting_cycles = 0;
            _status = READY;
        }
        else
        { // still waiting
            ++_waiting_cycles;
            dumpToFile();
        }
        break;

    case WORKING:
        //should never get here
        break;
    }
}

void CPU::tick()
{
    clk.tick();
}
} // namespace cpucore