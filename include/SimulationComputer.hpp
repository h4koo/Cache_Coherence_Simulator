#include <iostream>
#include <string>
// #include <fstream>
#include "omp.h"
#include "CPU.hpp"
#include "Bus.hpp"
#include "RAM.hpp"

#ifndef SIM_COMP
#define SIM_COMP

namespace simulationcomputer
{

const int NUM_CPUS = 4;

class SimulationComputer
{
private:
    bool _started;

    std::vector<cpucore::CPU> cpus;
    Bus bus;
    RAM ram;
    std::ofstream files[NUM_CPUS];

public:
    SimulationComputer()
    {
        printf("entered constructor");
        //create 4 cpus
        for (int i = 0; i < NUM_CPUS; ++i)
        {
            std::string cpu_id = "CPU00";
            cpu_id += std::to_string(i + 1);
            cpucore::CPU c(cpu_id);
            cpus.push_back(c);

            // std::string id = _id;
            cpu_id += ".txt";
            // _output_file.open(id);
            files[i].open(cpu_id);
            std::string terminal_command = "gnome-terminal -- sh -c 'tail -f /home/h4koo/Documents/Arqui2/Cache_Coherence_Simulator/build/bin/";
            terminal_command += cpu_id;
            terminal_command += "'";
            system(terminal_command.c_str());

            //connect them to the bus
            // cpus[i].connectBusPort(bus.connectCPU((Observer *)cpus[i].getCacheController()));
        }
        for (int i = 0; i < NUM_CPUS; ++i)
        {
            // std::string cpu_id = "CPU00";
            // cpu_id += std::to_string(i + 1);
            // cpucore::CPU c(cpu_id);
            // cpus.[i] = c;

            //their connected in a different loop because their address change after more items are pushed_back() in the cpus vector

            //connect them to the bus
            cpus[i].connectBusPort(bus.connectCPU((Observer *)cpus[i].getCacheController()));
            cpus[i].setOutputFile(&files[i]);
        }

        //connect RAM memory to bus
        ram.connectBusPort(bus.connectRAM((RAMObserver *)&ram));
    };

    ~SimulationComputer()
    {
        for (int i = 0; i < NUM_CPUS; ++i)
        {
            files[i].close();
        }
    }

    bool isStarted() { return _started; }

    void loopCPUs()
    {
        printf("started looping CPUs\n");
        while (_started)
        {
#pragma omp parallel num_threads(4) shared(cpus, bus, ram)
            {
                int thread_id = omp_get_thread_num();
                // printf("Looping thread%d\n", thread_id);
                cpus[thread_id].work();

#pragma omp barrier
                cpus[thread_id].tick();
            }
        }
    };

    void start()
    {
        _started = true;
        bus.start();
        printf("after starting bus\n");

        loopCPUs();
    };

    void stop()
    {
        _started = false;
        bus.stop();
    };
};

} // namespace simulationcomputer

#endif