#ifndef PARTICLES_HDR
#define PARTICLES_HDR

#include <random>
#include <vector>

#include "data_types.h"

struct Particles {
    private:
        std::vector<double> x_;
        std::vector<double> y_;
        std::vector<double> z_;
        std::vector<double> vx_;
        std::vector<double> vy_;
        std::vector<double> vz_;
        const double box_size_;
        const double sigma_velocity_ {1.0f};
    public:
        Particles(int nr_particles, double box_size, double sigma_velocity, unsigned long seed);
        const position_t position(size_t i) const { return {x_[i], y_[i], z_[i]}; }
        void position(size_t i, const position_t& pos) { x_[i] = pos[0]; y_[i] = pos[1]; z_[i] = pos[2]; }
        const velocity_t velocity(size_t i) const { return {vx_[i], vy_[i], vz_[i]}; }
        void velocity(size_t i, const velocity_t& vel) { vx_[i] = vel[0]; vy_[i] = vel[1]; vz_[i] = vel[2]; }
        size_t size() const { return x_.size(); }
        double box_size() const { return box_size_; }
        double sigma_velocity() const { return sigma_velocity_; }
};

std::ostream& operator<<(std::ostream& os, const Particles& particles);

#endif
