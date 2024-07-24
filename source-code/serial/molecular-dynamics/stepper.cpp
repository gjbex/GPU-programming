#include "stepper.h"


force_t Stepper::compute_force(const position_t& pos1, const position_t& pos2) {
    const double dx = pos2[0] - pos1[0];
    const double dy = pos2[1] - pos1[1];
    const double dz = pos2[2] - pos1[2];
    const double r2 = dx*dx + dy*dy + dz*dz;
    const double r6 = r2*r2*r2;
    const double r12 = r6*r6;
    const double f = 24.0f*epsilon_*(2.0f*sigma12_/r12 - sigma6_/r6)/r2;
    return {f*dx, f*dy, f*dz};
}

void Stepper::compute_forces(const Particles& particles) {
    for (size_t i = 0; i < particles.size(); ++i) {
        fx_[i] = 0.0;
        fy_[i] = 0.0;
        fz_[i] = 0.0;
    }
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            const auto force = compute_force(particles.position(i), particles.position(j));
            fx_[i] += force[0];
            fy_[i] += force[1];
            fz_[i] += force[2];
            fx_[j] -= force[0];
            fy_[j] -= force[1];
            fz_[j] -= force[2];
        }
    }
}

void Stepper::update_positions(Particles& particles, double delta_t) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const auto pos {particles.position(i)};
        const auto vel {particles.velocity(i)};
        const double x {pos[0] + vel[0]*delta_t};
        const double y {pos[1] + vel[1]*delta_t};
        const double z {pos[2] + vel[2]*delta_t};
        particles.position(i, {x, y, z});
    }
}

void Stepper::update_velocities(Particles& particles, double delta_t) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const auto vel {particles.velocity(i)};
        const double vx {vel[0] + fx_[i]*delta_t/mass_};
        const double vy {vel[1] + fy_[i]*delta_t/mass_};
        const double vz {vel[2] + fz_[i]*delta_t/mass_};
        particles.velocity(i, {vx, vy, vz});
    }
}

Stepper::Stepper(Particles& particles, double mass, double epsilon, double sigma, double delta_t) :
    particles_{particles}, mass_{mass}, epsilon_{epsilon}, sigma_{sigma}, delta_t_{delta_t} {
        fx_.resize(particles.size(), 0.0);
        fy_.resize(particles.size(), 0.0);
        fz_.resize(particles.size(), 0.0);
    }

void Stepper::step() {
    compute_forces(particles_);
    update_positions(particles_, delta_t_);
    compute_forces(particles_);
    update_velocities(particles_, delta_t_);
}
