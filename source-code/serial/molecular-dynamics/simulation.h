#ifndef SIMULATION_HDR
#define SIMULATION_HDR

#include <vector>

#include "abstract_measures.h"
#include "particles.h"
#include "stepper.h"

struct Simulation {
    private:
        Particles particles_;
        double t_ {0.0f};
        Stepper stepper_;
        double delta_t_;
        std::vector<MeasureBase*> measures_;

    public:
        Simulation(Particles& particles, Stepper& stepper) :
            particles_{particles}, stepper_{stepper}, delta_t_{stepper_.delta_t()} {}
        template <typename T>
        void add_measure(Measure<T>* measure) {
            measures_.push_back(new MeasureWrapper<T>(measure));
        }
        void run(int nr_steps, int measure_steps = 1, bool verbose = true);
        double time() const { return t_; }
};

#endif
