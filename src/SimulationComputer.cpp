#include "../include/SimulationComputer.hpp"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
// #include <iostream>
// #include <string>
// #include <omp.h>
// #include "../include/CPU.hpp"
// #include "../include/Bus.hpp"
// #include "../include/RAM.hpp"

using namespace simulationcomputer;

int main()
{

    printf("\nStarting Simulation\n");
    SimulationComputer comp;
    printf("after computer creation\n");
    std::thread *last_running_thread;
    // comp.start();
    std::thread t(&SimulationComputer::start, &comp);

    last_running_thread = &t;

    bool runnin_sim = true;
    while (runnin_sim)
    {
        char input;
        std::cin >> input;

        switch (input)
        {
            //to quit the simulation
        case 'q':
            printf("\nQuitting the program\n");
            if (comp.isStarted())
            {
                comp.stop();
                if (last_running_thread->joinable())
                    last_running_thread->join();
            }
            runnin_sim = false;
            break;
        case 'Q':
            printf("\nQuitting the program\n");
            if (comp.isStarted())
            {
                comp.stop();
                if (last_running_thread->joinable())
                    last_running_thread->join();
            }
            runnin_sim = false;
            break;
        case 'p':
            printf("\npausing execution of the simulation\n");
            if (comp.isStarted())
            {
                comp.stop();
                if (last_running_thread->joinable())
                    last_running_thread->join();
            }
            break;
        case 'P':
            printf("\npausing execution of the simulation\n");
            if (comp.isStarted())
            {
                comp.stop();
                if (last_running_thread->joinable())
                    last_running_thread->join();
            }
            break;
        case 's':
        {
            printf("\nrestarting execution of the simulation\n");
            if (!comp.isStarted())
            {
                t = std::thread(&SimulationComputer::start, &comp);
                last_running_thread = &t;
            }
            break;
        }
        case 'S':

        {
            printf("\nrestarting execution of the simulation\n");
            if (!comp.isStarted())
            {
                t = std::thread(&SimulationComputer::start, &comp);
                last_running_thread = &t;
            }
            break;
        }
        break;
        default:
            break;
        }
    }
    if (last_running_thread->joinable())
        last_running_thread->join();

    return 0;
};