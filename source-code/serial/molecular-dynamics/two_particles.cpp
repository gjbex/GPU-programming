#include <array>
#include <format>
#include <iostream>

#include "abstract_measures.h"
#include "cli.h"
#include "data_types.h"
#include "measures.h"
#include "particles.h"
#include "simulation.h"
#include "stepper.h"

using particle_data_t = std::array<double, 2>;

struct ParticleMeasure : public Measure<particle_data_t> {
    protected:
        const size_t particle_id_;
    public:
        ParticleMeasure(const Particles& particles, size_t particle_id) :
            Measure<particle_data_t>(
                    "x_" + std::to_string(particle_id) + " v_" + std::to_string(particle_id),
                    particles
                    ),
            particle_id_{particle_id} {}
        particle_data_t compute_value() const override {
            return {particles_.position(particle_id_)[0], particles_.velocity(particle_id_)[0]};
        }
        std::string current_value() const {
            return std::format("{:.5e} {:.5e}", values_.back()[0], values_.back()[1]);
        }
};

int main(int argc, char* argv[]) {
    command_line_args defaults {
        .nr_particles = 2,
        .box_size = 1.0,
        .nr_steps = 1'000,
        .measure_steps = 1,
        .delta_t = 1.0e-3,
        .mass = 1.0,
        .epsilon = 1.0,
        .sigma = 1.0,
        .temperature = 300.0,
        .velocity_sigma = 1.0,
        .seed = 1234
    };
    command_line_args args = parse_command_line(argc, argv, defaults);
    // create particles
    Particles particles(args.nr_particles, args.box_size, args.velocity_sigma, args.seed);
    particles.position(0, {-0.6, 0.0, 0.0});
    particles.position(1, {0.6, 0.0, 0.0});
    particles.velocity(0, {0.0, 0.0, 0.0});
    particles.velocity(1, {0.0, 0.0, 0.0});
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
    // add particle measures
    ParticleMeasure particle0(particles, 0);
    simulation.add_measure(&particle0);
    ParticleMeasure particle1(particles, 1);
    simulation.add_measure(&particle1);
    // run simulation
    simulation.run(args.nr_steps, args.measure_steps);
    return 0;
}
