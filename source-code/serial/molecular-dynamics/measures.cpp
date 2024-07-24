#include <algorithm>
#include <cmath>
#include <format>
#include <vector>

#include "data_types.h"
#include "measures.h"

double KineticEnergy::compute_value() const {
    double v2 {0.0f};
    for (size_t i = 0; i < particles_.size(); ++i) {
        const auto vel {particles_.velocity(i)};
        v2 += vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
    }
    return 0.5f*mass_*v2;
}

std::string KineticEnergy::current_value() const {
    return std::format("{:.5e}", values_.back());
}

double PotentialEnergy::compute_value() const {
    double energy = 0.0f;
    for (size_t i = 0; i < particles_.size(); ++i) {
        for (size_t j = i + 1; j < particles_.size(); ++j) {
            energy += potential_energy(particles_.position(i), particles_.position(j));
        }
    }
    return 2.0f*energy;
}

std::string PotentialEnergy::current_value() const {
    return std::format("{:.5e}", values_.back());
}

std::string TotalEnergy::current_value() const {
    return std::format("{:.5e}", values_.back());
}

position_t CenterOfMass::compute_value() const {
    position_t com = {0.0f, 0.0f, 0.0f};
    for (size_t i = 0; i < particles_.size(); ++i) {
        const auto pos = particles_.position(i);
        com[0] += pos[0];
        com[1] += pos[1];
        com[2] += pos[2];
    }
    com[0] /= particles_.size();
    com[1] /= particles_.size();
    com[2] /= particles_.size();
    return com;
}

std::string CenterOfMass::current_value() const {
    const auto& value = values_.back();
    return std::format("{:.5e}", value[0]) +
        " " + std::format("{:.5e}", value[1]) +
        " " + std::format("{:.5e}", value[2]);
}

std::vector<double> DistanceDistribution::compute_value() const {
    std::vector<double> distances;
    for (size_t i = 0; i < particles_.size(); ++i) {
        for (size_t j = i + 1; j < particles_.size(); ++j) {
            const auto pos1 = particles_.position(i);
            const auto pos2 = particles_.position(j);
            const double dx = pos2[0] - pos1[0];
            const double dy = pos2[1] - pos1[1];
            const double dz = pos2[2] - pos1[2];
            const double r = sqrt(dx*dx + dy*dy + dz*dz);
            distances.push_back(r);
        }
    }
    std::sort(distances.begin(), distances.end());
    return distances;
}

std::string DistanceDistribution::current_value() const {
    std::string value;
    for (const auto& dist : values_.back()) {
        value += std::format("{:.5e}", dist) + ',';
    }
    return value;
}
