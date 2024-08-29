#include "data_types.h"

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& pos) {
    os << pos[0] << ' ' << pos[1] << ' ' << pos[2];
    return os;
}
