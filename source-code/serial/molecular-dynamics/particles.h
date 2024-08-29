#ifndef PARTICLES_HDR
#define PARTICLES_HDR

#include <random>
#include <vector>

#include "data_types.h"

struct Particles {
    private:
        std::vector<double> pos_;
        std::vector<double> vel_;
        const double box_size_;
        const double sigma_velocity_ {1.0f};
    public:
        Particles(int nr_particles, double box_size, double sigma_velocity, unsigned long seed);
        const position_t position(size_t i) const { return {pos_[3*i], pos_[3*i + 1], pos_[3*i + 2]}; }
        void position(size_t i, const position_t& pos) {
            pos_[3*i] = pos[0];
            pos_[3*i + 1] = pos[1];
            pos_[3*i + 2] = pos[2];
        }
        const velocity_t velocity(size_t i) const { return {vel_[3*i], vel_[3*i + 1], vel_[3*i + 2]}; }
        void velocity(size_t i, const velocity_t& vel) {
            vel_[3*i] = vel[0];
            vel_[3*i + 1] = vel[1];
            vel_[3*i + 2] = vel[2]; }
        size_t size() const { return pos_.size()/3; }
        double box_size() const { return box_size_; }
        double sigma_velocity() const { return sigma_velocity_; }
};

std::ostream& operator<<(std::ostream& os, const Particles& particles);

#endif
