#include <iostream>

#include "cli.h"
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
    /*
    CenterOfMass center_of_mass(particles);
    simulation.add_measure(&center_of_mass);
    */
    DistanceDistributionStats distance_stats(particles);
    simulation.add_measure(&distance_stats);
    VelocityDistributionStats velocity_stats(particles);
    simulation.add_measure(&velocity_stats);
    ForceDistributionStats force_stats(particles, args.epsilon, args.sigma);
    simulation.add_measure(&force_stats);
    // run simulation
    simulation.run(args.nr_steps, args.measure_steps);
    return 0;
}
