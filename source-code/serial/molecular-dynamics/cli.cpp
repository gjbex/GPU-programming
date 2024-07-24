#include <iostream>

#include "cli.h"

command_line_args parse_command_line(int argc, char* argv[]) {
    namespace po = boost::program_options;
    command_line_args args;
    po::options_description desc("Allowed options");
    // default values are for CO_2
    desc.add_options()
        ("help", "produce help message")
        ("nr-particles", po::value<int>(&args.nr_particles)->default_value(args.nr_particles),
         "number of particles")
        ("box-size", po::value<double>(&args.box_size)->default_value(args.box_size),
         "box size for initial positions (m)")
        ("nr-steps", po::value<int>(&args.nr_steps)->default_value(args.nr_steps),
         "number of steps")
        ("measure_steps", po::value<int>(&args.measure_steps)->default_value(args.measure_steps),
         "number of measurement steps")
        ("delta-t", po::value<double>(&args.delta_t)->default_value(args.delta_t),
         "time step (s)")
        ("mass", po::value<double>(&args.mass)->default_value(args.mass),
         "particle mass (kg)")
        ("epsilon", po::value<double>(&args.epsilon)->default_value(args.epsilon),
         "Lennard-Jones epsilon (J)")
        ("sigma", po::value<double>(&args.sigma)->default_value(args.sigma),
         "Lennard-Jones sigma (m)")
        ("velocity_sigma", po::value<double>(&args.velocity_sigma)->default_value(args.velocity_sigma),
         "velocity sigma (m/s)")
        ("temperature", po::value<double>(&args.temperature)->default_value(args.temperature),
         "temperature (K)")
        ("seed", po::value<unsigned long>(&args.seed)->default_value(args.seed),
         "random seed");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
    return args;
}
