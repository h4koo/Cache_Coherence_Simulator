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
        ++_waiting_cycles;
        std::thread cache_work(&CPU::readMem, this);
        cache_work.detach();
        // std::async(std::launch::async, &CPU::readMem, this);

        dumpToFile();

        break;
    }
    case WRITE_MEM:
    {
        _status = WAITING;
        //called as an asynchronous job
        // std::async(std::launch::async, &CPU::writeMem, this);
        ++_waiting_cycles;
        std::thread cache_work(&CPU::writeMem, this);
        cache_work.detach();

        dumpToFile();
        break;
    }
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

    // printf("\n\n\nThe processor with ID: %s has run for %d cycles\n", _id.c_str(), _total_cycles);
    (*_output_file) << "\n\n\nThe processor with ID:" << _id.c_str() << " has run for " << _total_cycles << " cycles\n";
    switch (_status)
    {
    case READY:
        // printf("Is currently ready for a new task\n");
        (*_output_file) << "Is currently ready for a new task\n";
        break;
    case WORKING:
        // printf("Is currently executing a work instruction\n");
        (*_output_file) << "Is currently executing a work instruction\n";
        break;
    case WAITING:

        if (_current_instruction.inst_type == READ_MEM)
        {
            if (_waiting_cycles == 0) //finished reading
            {
                // printf("Processor has READ value: %s from address %d\n", _read_data.c_str(),                 _current_instruction.inst_address);
                (*_output_file) << "Processor has READ value:" << _read_data.c_str() << " from address " << _current_instruction.inst_address << "\n";

                //output contents of cache
                _cache.dumpCacheToFile(_output_file);
            }
            else
            {
                // printf("Is currently waiting on a READ operation to address %d\n", _current_instruction.inst_address);
                (*_output_file) << "Is currently waiting on a READ operation to address " << _current_instruction.inst_address << "\n";
            }
        }
        else if (_current_instruction.inst_type == WRITE_MEM)
        {
            // printf("Is currently waiting on a WRITE operation to address %d\n", _current_instruction.inst_address);
            (*_output_file) << "Is currently waiting on a WRITE operation to address " << _current_instruction.inst_address << "\n";
            if (_waiting_cycles == 0) //finished writing
            {
                // printf("Processor has finished writing to address %d\n", _current_instruction.inst_address);
                (*_output_file) << "Processor has finished writing to address " << _current_instruction.inst_address << "\n";

                //output contents of cache
                _cache.dumpCacheToFile(_output_file);
            }
        }

        break;
    }
    (*_output_file).flush();
    //     std::string curr_status = _status == READY ? "ready for a new instruction" : _status == WORKING? "executing instrucion":"waiting"
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

            //reset waiting cycle count
            _waiting_cycles = 0;

            dumpToFile();

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