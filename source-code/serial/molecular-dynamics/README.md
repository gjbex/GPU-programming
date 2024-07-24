# Molecular dynamics

This is a very naive molecular dynamics simulation that can
be used to experiment with various parallelization strategies and
GPU acceleration.

It is *not* intended to be used for production simulations, or even
as a basis for molecular dynamics applications.


## What is it?

1. `data_types.h`: definition of data types used in the implementation.
1. `data_types.cpp`: implementation of data types used in the implementation.
1. `simulation.h`: class definition for a simulation.
1. `simulation.cpp`: class implementation for a simulation.
1. `stepper.h`: class definition for a time stepper, i.e.,
   the dynamics of the particles.
1. `particles.h`: class definition for a collection of particles.
1. `particles.cpp`: class implementation for a collection of particles.
1. `stepper.cpp`: class implementation for a time stepper.
1. `abstract_measures.h`: template and class definition for
   measures that can be computed during the simulation.
1. `measures.h`: class definition for measures that can be computed
   during the simulation.
1. `measures.cpp`: class implementation for measures that can be
   computed during the simulation.
1. `cli.h`: command line interface declaration.
1. `cli.cpp`: command line interface implementation.
1. `main.cpp`: main program that sets up the simulation and runs it.
1. `CMakeLists.txt`: CMake file to build the application.
