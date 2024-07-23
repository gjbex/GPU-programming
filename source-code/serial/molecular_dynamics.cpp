#include <algorithm>
#include <array>
#include <boost/program_options.hpp>
#include <cmath>
#include <format>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using position_t = std::array<double, 3>;
using velocity_t = std::array<double, 3>;
using force_t = std::array<double, 3>;

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
        Particles(int nr_particles, double box_size, double sigma_velocity, unsigned long seed) :
            box_size_{box_size}, sigma_velocity_{sigma_velocity} {
            x_.reserve(nr_particles);
            y_.reserve(nr_particles);
            z_.reserve(nr_particles);
            vx_.reserve(nr_particles);
            vy_.reserve(nr_particles);
            vz_.reserve(nr_particles);
            std::mt19937 gen(seed);
            std::uniform_real_distribution<double> position_distr(-0.5f*box_size, 0.5f*box_size);
            for (int i = 0; i < nr_particles; ++i) {
                x_.push_back(position_distr(gen));
                y_.push_back(position_distr(gen));
                z_.push_back(position_distr(gen));
            }
            std::normal_distribution<double> velocity_distr(0.0f, sigma_velocity_);
            for (int i = 0; i < nr_particles; ++i) {
                vx_.push_back(velocity_distr(gen));
                vy_.push_back(velocity_distr(gen));
                vz_.push_back(velocity_distr(gen));
            }
        }
        const position_t position(size_t i) const { return {x_[i], y_[i], z_[i]}; }
        void position(size_t i, const position_t& pos) { x_[i] = pos[0]; y_[i] = pos[1]; z_[i] = pos[2]; }
        const velocity_t velocity(size_t i) const { return {vx_[i], vy_[i], vz_[i]}; }
        void velocity(size_t i, const velocity_t& vel) { vx_[i] = vel[0]; vy_[i] = vel[1]; vz_[i] = vel[2]; }
        size_t size() const { return x_.size(); }
        double box_size() const { return box_size_; }
        double sigma_velocity() const { return sigma_velocity_; }
};

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& pos) {
    os << pos[0] << ' ' << pos[1] << ' ' << pos[2];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Particles& particles) {
    os << "x y z vx vy vz" << std::endl;
    for (size_t i = 0; i < particles.size(); ++i) {
        os << particles.position(i) << ' ' << particles.velocity(i) << std::endl;
    }
    return os;
}

template <typename T>
struct Measure {
    protected:
        const std::string name_;
        const Particles& particles_;
        std::vector<T> values_;

    public:
        Measure(const std::string& name, const Particles& particles) :
            name_{name}, particles_{particles} {}
        std::string name() const { return name_; }
        virtual T compute_value() const = 0;
        void compute() {
            values_.push_back(compute_value());
        }
        virtual std::string current_value() const = 0;
        ~Measure() = default;
};

struct MeasureBase {
    public:
        virtual std::string name() const = 0;
        virtual void compute() = 0;
        virtual std::string current_value() const = 0;
        ~MeasureBase() = default;
};

template <typename T>
struct MeasureWrapper : public MeasureBase {
    protected:
        Measure<T>* measure_;
    public:
        explicit MeasureWrapper(Measure<T>* measure) : measure_{measure} {}
        std::string name() const override { return measure_->name(); }
        void compute() override { measure_->compute(); }
        std::string current_value() const override { return measure_->current_value(); }
        ~MeasureWrapper() {
            delete measure_;
        }
};

struct KineticEnergy : public Measure<double> {
    protected:
        const double mass_;

    public:
        explicit KineticEnergy(const Particles& particles, double mass) :
            Measure<double>("kinetic_energy", particles), mass_{mass} {}
        double compute_value() const override {
            double v2 {0.0f};
            for (size_t i = 0; i < particles_.size(); ++i) {
                const auto vel {particles_.velocity(i)};
                v2 += vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
            }
            return 0.5f*mass_*v2;
        }
        std::string current_value() const override {
            return std::format("{:.5e}", values_.back());
        }
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
        double compute_value() const override {
            double energy = 0.0f;
            for (size_t i = 0; i < particles_.size(); ++i) {
                for (size_t j = i + 1; j < particles_.size(); ++j) {
                    energy += potential_energy(particles_.position(i), particles_.position(j));
                }
            }
            return 2.0f*energy;
        }
        std::string current_value() const override {
            return std::format("{:.5e}", values_.back());
        }
};

struct CenterOfMass : public Measure<position_t> {
    public:
        explicit CenterOfMass(const Particles& particles) :
            Measure<position_t>("center_of_mass", particles) {}
        position_t compute_value() const override {
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
        std::string current_value() const override {
            const auto& value = values_.back();
            return std::format("{:.5e}", value[0]) +
                "," + std::format("{:.5e}", value[1]) +
                "," + std::format("{:.5e}", value[2]);
        }

};

struct DistanceDistribution : public Measure<std::vector<double>> {
    public:
        explicit DistanceDistribution(const Particles& particles) :
            Measure<std::vector<double>>("distance_distribution", particles) {}
        std::vector<double> compute_value() const override {
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
        std::string current_value() const override {
            std::string value;
            for (const auto& dist : values_.back()) {
                value += std::format("{:.5e}", dist) + ',';
            }
            return value;
        }
};

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

        force_t compute_force(const position_t& pos1, const position_t& pos2) {
            const double dx = pos2[0] - pos1[0];
            const double dy = pos2[1] - pos1[1];
            const double dz = pos2[2] - pos1[2];
            const double r2 = dx*dx + dy*dy + dz*dz;
            const double r6 = r2*r2*r2;
            const double r12 = r6*r6;
            const double f = 24.0f*epsilon_*(2.0f*sigma12_/r12 - sigma6_/r6)/r2;
            return {f*dx, f*dy, f*dz};
        }
        void compute_forces(const Particles& particles) {
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
        void update_positions(Particles& particles, double delta_t) {
            for (size_t i = 0; i < particles.size(); ++i) {
                const auto pos {particles.position(i)};
                const auto vel {particles.velocity(i)};
                const double x {pos[0] + vel[0]*delta_t};
                const double y {pos[1] + vel[1]*delta_t};
                const double z {pos[2] + vel[2]*delta_t};
                particles.position(i, {x, y, z});
            }
        }
        void update_velocities(Particles& particles, double delta_t) {
            for (size_t i = 0; i < particles.size(); ++i) {
                const auto vel {particles.velocity(i)};
                const double vx {vel[0] + fx_[i]*delta_t/mass_};
                const double vy {vel[1] + fy_[i]*delta_t/mass_};
                const double vz {vel[2] + fz_[i]*delta_t/mass_};
                particles.velocity(i, {vx, vy, vz});
            }
        }

    public:
        Stepper(Particles& particles, double mass, double epsilon, double sigma, double delta_t) :
            particles_{particles}, mass_{mass}, epsilon_{epsilon}, sigma_{sigma}, delta_t_{delta_t} {
            fx_.resize(particles.size(), 0.0);
            fy_.resize(particles.size(), 0.0);
            fz_.resize(particles.size(), 0.0);
        }
        void step() {
            compute_forces(particles_);
            update_positions(particles_, delta_t_);
            compute_forces(particles_);
            update_velocities(particles_, delta_t_);
        }
        double delta_t() const { return delta_t_; }
};

struct Simulation {
    private:
        Particles particles_;
        double t_ {0.0f};
        double delta_t_;
        Stepper stepper_;
        std::vector<MeasureBase*> measures_;

    public:
        Simulation(Particles& particles, Stepper& stepper) :
            particles_{particles}, stepper_{stepper} {
                delta_t_ = stepper_.delta_t();
            }
        template <typename T>
        void add_measure(Measure<T>* measure) {
            measures_.push_back(new MeasureWrapper<T>(measure));
        }
        void run(int nr_steps, int measure_steps = 1, bool verbose = true) {
            if (verbose) {
                std::cout << "time";
                for (auto& measure : measures_) {
                    std::cout << ' ' << measure->name();
                }
                std::cout << std::endl;
            }
            for (int step = 0; step < nr_steps; ++step) {
                if (verbose && step % measure_steps == 0) {
                    std::cout << std::scientific << std::setprecision(6);
                    std::cout << t_ << std::scientific;
                }
                for (auto& measure : measures_) {
                    if (step % measure_steps == 0) {
                        measure->compute();
                        if (verbose) {
                            std::cout << ' ' << measure->current_value();
                        }
                    }
                }
                if (verbose && step % measure_steps == 0) {
                    std::cout << std::endl;
                    std::cout << std::defaultfloat;
                }
                stepper_.step();
                t_ += delta_t_;
            }
        }
        double time() const { return t_; }
};

struct command_line_args {
    int nr_particles;
    double box_size;
    int nr_steps;
    int measure_steps;
    double delta_t;
    double mass;
    double epsilon;
    double sigma;
    double temperature;
    double velocity_sigma;
    unsigned long seed;
};

namespace po = boost::program_options;

command_line_args parse_command_line(int argc, char* argv[]) {
    namespace po = boost::program_options;
    command_line_args args;
    po::options_description desc("Allowed options");
    // default values are for CO_2
    desc.add_options()
        ("help", "produce help message")
        ("nr-particles", po::value<int>(&args.nr_particles)->default_value(10), "number of particles")
        ("box-size", po::value<double>(&args.box_size)->default_value(1.0e-6f), "box size for initial positions (m)")
        ("nr-steps", po::value<int>(&args.nr_steps)->default_value(10), "number of steps")
        ("measure_steps", po::value<int>(&args.measure_steps)->default_value(1), "number of measurement steps")
        ("delta-t", po::value<double>(&args.delta_t)->default_value(1.0e-6f), "time step (s)")
        ("mass", po::value<double>(&args.mass)->default_value(7.3e-26f), "particle mass (kg)")
        ("epsilon", po::value<double>(&args.epsilon)->default_value(256.0f*1.38e-23f), "Lennard-Jones epsilon (J)")
        ("sigma", po::value<double>(&args.sigma)->default_value(0.37e-9f), "Lennard-Jones sigma (m)")
        ("velocity_sigma", po::value<double>(&args.velocity_sigma)->default_value(3.2e2f), "velocity sigma (m/s)")
        ("temperature", po::value<double>(&args.temperature)->default_value(300.0), "temperature (K)")
        ("seed", po::value<unsigned long>(&args.seed)->default_value(1234), "random seed");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
    return args;
}

int main(int argc, char* argv[]) {
    command_line_args args = parse_command_line(argc, argv);
    // create particles
    Particles particles(args.nr_particles, args.box_size, args.velocity_sigma, args.seed);
    // create stepper and simulation
    Stepper stepper(particles, args.mass, args.epsilon, args.sigma, args.delta_t);
    Simulation simulation(particles, stepper);
    // define measures and add to simulation
    KineticEnergy kinetic_energy(particles, args.mass);
    simulation.add_measure(&kinetic_energy);
    PotentialEnergy potential_energy(particles, args.epsilon, args.sigma);
    simulation.add_measure(&potential_energy);
    CenterOfMass center_of_mass(particles);
    simulation.add_measure(&center_of_mass);
    // run simulation
    simulation.run(args.nr_steps, args.measure_steps);
    return 0;
}
