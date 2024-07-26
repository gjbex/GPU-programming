#include "stepper.h"

force_t Stepper::compute_force(const position_t& pos1, const position_t& pos2) {
    const double dx = pos2[0] - pos1[0];
    const double dy = pos2[1] - pos1[1];
    const double dz = pos2[2] - pos1[2];
    const double r2 = dx*dx + dy*dy + dz*dz;
    const double r6 = r2*r2*r2;
    const double r12 = r6*r6;
    const double f = -24.0f*epsilon_*(2.0f*sigma12_/r12 - sigma6_/r6)/r2;
    return {f*dx, f*dy, f*dz};
}

void Stepper::compute_forces(const Particles& particles) {
    for (size_t i = 0; i < f_.size(); ++i) {
        f_[i] = 0.0;
    }
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            const auto force = compute_force(particles.position(i), particles.position(j));
            for (size_t k = 0; k < 3; ++k) {
                f_[3*i + k] += force[k];
                f_[3*j + k] -= force[k];
            }
        }
    }
}

void Stepper::update_positions(Particles& particles, double delta_t) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const auto pos {particles.position(i)};
        const auto vel {particles.velocity(i)};
        position_t new_pos;
        for (size_t k = 0; k < 3; ++k) {
            new_pos[k] = pos[k] + vel[k]*delta_t;
        }
        particles.position(i, new_pos);
    }
}

void Stepper::update_velocities(Particles& particles, double delta_t) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const auto vel {particles.velocity(i)};
        velocity_t new_vel;
        for (size_t k = 0; k < 3; ++k) {
            new_vel[k] = vel[k] + f_[3*i + k]*delta_t/mass_;
        }
        particles.velocity(i, new_vel);
    }
}

Stepper::Stepper(Particles& particles, double mass, double epsilon, double sigma, double delta_t) :
    particles_{particles}, mass_{mass}, epsilon_{epsilon}, sigma_{sigma}, delta_t_{delta_t} {
        f_.resize(3*particles.size(), 0.0);
    }

void Stepper::step() {
    update_positions(particles_, delta_t_);
    compute_forces(particles_);
    update_velocities(particles_, delta_t_);
}
