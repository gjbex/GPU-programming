#ifndef MEASURES_HDR
#define MEASURES_HDR

#include "abstract_measures.h"

struct KineticEnergy : public Measure<double> {
    protected:
        const double mass_;

    public:
        explicit KineticEnergy(const Particles& particles, double mass) :
            Measure<double>("kinetic_energy", particles), mass_{mass} {}
        double compute_value() const override;
        std::string current_value() const override;
};

struct PotentialEnergy : public Measure<double> {
    protected:
        double epsilon_;
        double sigma_;
        double sigma6_ {sigma_*sigma_*sigma_*sigma_*sigma_*sigma_};
        double sigma12_ {sigma6_*sigma6_};
        double potential_energy(const position_t& pos1, const position_t& pos2) const {
            const double dx {pos2[0] - pos1[0]};
            const double dy {pos2[1] - pos1[1]};
            const double dz {pos2[2] - pos1[2]};
            const double r2 {dx*dx + dy*dy + dz*dz};
            const double r6 {r2*r2*r2};
            const double r12 {r6*r6};
            return 4.0f*epsilon_*(sigma12_/r12 - sigma6_/r6);
        }

    public:
        explicit PotentialEnergy(const Particles& particles, double epsilon, double sigma) :
            Measure<double>("potential_energy", particles), epsilon_{epsilon}, sigma_{sigma} {}
        double compute_value() const override;
        std::string current_value() const override;
};

struct TotalEnergy : public Measure<double> {
    protected:
        const KineticEnergy& kinetic_energy_;
        const PotentialEnergy& potential_energy_;

    public:
        TotalEnergy(const Particles& particles, const KineticEnergy& kinetic_energy,
                const PotentialEnergy& potential_energy) :
            Measure<double>("total_energy", particles), kinetic_energy_{kinetic_energy},
            potential_energy_{potential_energy} {}
        double compute_value() const override {
            return kinetic_energy_.get_value(values_.size()) + potential_energy_.get_value(values_.size());
        }
        std::string current_value() const override;
};

struct CenterOfMass : public Measure<position_t> {
    public:
        explicit CenterOfMass(const Particles& particles) :
            Measure<position_t>("cm_x cm_y cm_z", particles) {}
        position_t compute_value() const override;
        std::string current_value() const override;
};

struct DistanceDistribution : public Measure<std::vector<double>> {
    public:
        explicit DistanceDistribution(const Particles& particles) :
            Measure<std::vector<double>>("distance_distribution", particles) {}
        std::vector<double> compute_value() const override;
        std::string current_value() const override;
};

#endif
