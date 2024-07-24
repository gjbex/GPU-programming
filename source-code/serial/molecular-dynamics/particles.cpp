#include "particles.h"

Particles::Particles(int nr_particles, double box_size, double sigma_velocity, unsigned long seed) :
    box_size_{box_size}, sigma_velocity_{sigma_velocity} {
        pos_.reserve(3*nr_particles);
        vel_.reserve(3*nr_particles);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<double> position_distr(-0.5f*box_size, 0.5f*box_size);
        for (int i = 0; i < nr_particles; ++i) {
            pos_.push_back(position_distr(gen));
            pos_.push_back(position_distr(gen));
            pos_.push_back(position_distr(gen));
        }
        std::normal_distribution<double> velocity_distr(0.0f, sigma_velocity_);
        for (int i = 0; i < nr_particles; ++i) {
            vel_.push_back(velocity_distr(gen));
            vel_.push_back(velocity_distr(gen));
            vel_.push_back(velocity_distr(gen));
        }
    }

std::ostream& operator<<(std::ostream& os, const Particles& particles) {
    os << "x y z vx vy vz" << std::endl;
    for (size_t i = 0; i < particles.size(); ++i) {
        os << particles.position(i) << ' ' << particles.velocity(i) << std::endl;
    }
    return os;
}
