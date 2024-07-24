#ifndef STEPPER_HDR
#define STEPPER_HDR

#include "particles.h"

struct Stepper {
    private:
        Particles& particles_;
        const double mass_;
        const double epsilon_;
        const double sigma_;
        const double sigma6_ {sigma_*sigma_*sigma_*sigma_*sigma_*sigma_};
        const double sigma12_ {sigma6_*sigma6_};
        const double delta_t_;
        std::vector<double> fx_;
        std::vector<double> fy_;
        std::vector<double> fz_;

        force_t compute_force(const position_t& pos1, const position_t& pos2);
        void compute_forces(const Particles& particles);
        void update_positions(Particles& particles, double delta_t);
        void update_velocities(Particles& particles, double delta_t);

    public:
        Stepper(Particles& particles, double mass, double epsilon, double sigma, double delta_t);
        void step();
        double delta_t() const { return delta_t_; }
};

#endif
