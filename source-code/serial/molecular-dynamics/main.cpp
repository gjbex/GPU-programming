#include <iostream>

#include "abstract_measures.h"
#include "cli.h"
#include "data_types.h"
#include "measures.h"
#include "particles.h"
#include "simulation.h"
#include "stepper.h"

int main(int argc, char* argv[]) {
    command_line_args args = parse_command_line(argc, argv);
    // create particles
    Particles particles(args.nr_particles, args.box_size, args.velocity_sigma, args.seed);
    // create stepper and simulation
    Stepper stepper(particles, args.mass, args.epsilon, args.sigma, args.delta_t);
    Simulation simulation(particles, stepper);
    // define measures and add to simulation
    KineticEnergy kinetic_energy(particles, args.mass);
    simulation.add_measure(&kinetic_energy);
    PotentialEnergy potential_energy(particles, args.epsilon, args.sigma);
    simulation.add_measure(&potential_energy);
    TotalEnergy total_energy(particles, kinetic_energy, potential_energy);
    simulation.add_measure(&total_energy);
    CenterOfMass center_of_mass(particles);
    simulation.add_measure(&center_of_mass);
    // run simulation
    simulation.run(args.nr_steps, args.measure_steps);
    return 0;
}
