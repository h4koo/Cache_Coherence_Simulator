#include "../include/SimulationComputer.hpp"
// #include <iostream>
// #include <string>
// #include <omp.h>
// #include "../include/CPU.hpp"
// #include "../include/Bus.hpp"
// #include "../include/RAM.hpp"

using namespace simulationcomputer;

// const int NUM_CPUS = 4;

// class SimulationComputer
// {
//     bool _started;

//     std::vector<cpucore::CPU> cpus;
//     Bus bus;
//     RAM ram;

// public:
//     SimulationComputer()
//     {
//         //create 4 cpus
//         for (int i = 0; i < NUM_CPUS; ++i)
//         {
//             std::string cpu_id = "CPU00";
//             cpu_id += std::to_string(i);
//             cpucore::CPU c(cpu_id);
//             cpus.push_back(c);

//             //connect them to the bus
//             bus.connectCPU((Observer *)&(cpus[i]));
//         }

//         //connect RAM memory to bus
//         bus.connectRAM((RAMObserver *)&ram);
//     };

//     void loopCPUs()
//     {
//         while (_started)
//         {
// #pragma omp parallel num_threads(4) shared(cpus, bus, ram)
//             {
//                 int thread_id = omp_get_thread_num();
//                 cpus[thread_id].work();

// #pragma omp barrier
//                 cpus[thread_id].tick();
//             }
//         }
//     };

//     void start()
//     {
//         _started = true;
//         loopCPUs();
//     };

//     void stop()
//     {
//         _started = false;
//     };
// };

int main()
{
    printf("\nStarting Simulation\n");
    SimulationComputer comp;
    printf("after computer creation\n");

    comp.start();
    printf("after computer start\n");
};