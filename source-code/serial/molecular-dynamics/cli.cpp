#include <iostream>

#include "cli.h"

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
        ("delta-t", po::value<double>(&args.delta_t)->default_value(1.0e-9f), "time step (s)")
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
