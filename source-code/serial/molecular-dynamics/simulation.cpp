#include <iostream>

#include "simulation.h"

void Simulation::run(int nr_steps, int measure_steps, bool verbose) {
    if (verbose) {
        std::cout << "time";
        for (auto& measure : measures_) {
            std::cout << ' ' << measure->name();
        }
        std::cout << std::endl;
    }
    for (int step = 0; step < nr_steps; ++step) {
        if (verbose && step % measure_steps == 0) {
            std::cout << t_;
        }
        for (auto& measure : measures_) {
            if (step % measure_steps == 0) {
                measure->compute();
                if (verbose) {
                    std::cout << ' ' << measure->current_value();
                }
            }
        }
        if (verbose && step % measure_steps == 0) {
            std::cout << std::endl;
        }
        stepper_.step();
        t_ += delta_t_;
    }
}
