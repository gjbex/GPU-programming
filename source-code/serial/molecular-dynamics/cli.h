#ifndef CLI_HDR
#define CLI_HDR

#include <boost/program_options.hpp>

struct command_line_args {
    int nr_particles {100};
    double box_size {1.0e-6};
    int nr_steps {10};
    int measure_steps {1};
    double delta_t {1.0e-12};
    double mass {7.3e-26};
    double epsilon {256.0*1.38e-23};
    double sigma {0.37e-9};
    double velocity_sigma {3.2e2};
    unsigned long seed {1234};
};

namespace po = boost::program_options;

command_line_args parse_command_line(int argc, char* argv[]);
command_line_args parse_command_line(int argc, char* argv[], const command_line_args& defaults);

#endif
