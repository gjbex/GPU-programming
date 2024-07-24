#ifndef CLI_HDR
#define CLI_HDR

#include <boost/program_options.hpp>

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

command_line_args parse_command_line(int argc, char* argv[]);

#endif
