#ifndef DATA_TYPES_HDR
#define DATA_TYPES_HDR

#include <array>
#include <iostream>

using position_t = std::array<double, 3>;
using velocity_t = std::array<double, 3>;
using force_t = std::array<double, 3>;

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& pos);

#endif
