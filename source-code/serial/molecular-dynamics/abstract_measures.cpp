#include <format>

#include "abstract_measures.h"

std::string ScalarMeasure::current_value() const {
    return std::format("{:.5e}", values_.back());
}

std::string VectorMeasure::current_value() const {
    const auto& value = values_.back();
    return std::format("{:.5e}", value[0]) +
        " " + std::format("{:.5e}", value[1]) +
        " " + std::format("{:.5e}", value[2]);
}

std::string StatisticsMeasure::current_value() const {
    const auto& value = values_.back();
    return std::format("{:.5e}", value.mean) +
        " " + std::format("{:.5e}", value.std_dev) +
        " " + std::format("{:.5e}", value.min) +
        " " + std::format("{:.5e}", value.max);
}
