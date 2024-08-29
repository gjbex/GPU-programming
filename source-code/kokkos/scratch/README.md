# Scratch pad

These application illustrate the use of Kokkos scratch pad memory.


## What is it?

1. `contract_data_field_scalar.cpp`: simple contract example using scalar
   fields, no scratch pad memory.
1. `contract_data_field_scalar_mdrange.cpp`: simple contract example using
   an `MDRangePolicy`, no scratch pad memory.
1. `contract_data_field_scalar_teams_vector.cpp`: simple contract example using
   `TeamPolicy`, `TeamThreadRange`, and `ThreadVectorRange`, no scratch pad.
1. `contract_data_field_scalar_teams.cpp`: simple contract example using
   `TeamPolicy`, no scratch pad memory.
1. `contract_data_field_scalar_scratch.cpp`: simple contract example using
   using scratch pad memory.
1. `CMakeLists.txt`: CMake file to build the applications.
