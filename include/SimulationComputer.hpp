#include <iostream>
#include <string>
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

            //connect them to the bus
            // cpus[i].connectBusPort(bus.connectCPU((Observer *)cpus[i].getCacheController()));
        }
        for (int i = 0; i < NUM_CPUS; ++i)
        {
            // std::string cpu_id = "CPU00";
            // cpu_id += std::to_string(i + 1);
            // cpucore::CPU c(cpu_id);
            // cpus.[i] = c;

            //connect them to the bus
            cpus[i].connectBusPort(bus.connectCPU((Observer *)cpus[i].getCacheController()));
        }

        //connect RAM memory to bus
        ram.connectBusPort(bus.connectRAM((RAMObserver *)&ram));
    };

    void loopCPUs()
    {
        printf("started looping CPUs\n");
        while (_started)
        {
// #pragma omp parallel num_threads(4) shared(cpus, bus, ram)
            {
                int thread_id = omp_get_thread_num();
                // printf("Looping thread%d\n", thread_id);
                cpus[thread_id].work();

// #pragma omp barrier
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
    };
};

} // namespace simulationcomputer

#endif