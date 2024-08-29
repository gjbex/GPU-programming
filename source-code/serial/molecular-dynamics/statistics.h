#ifndef STATISTICS_HDR
#define STATISTICS_HDR

#include <algorithm>
#include <cmath>
#include <limits>

struct stats_t {
    double mean;
    double std_dev;
    double min;
    double max;
};

struct Statistics {
    private:
        int count_ {0};
        double sum_ {0.0f};
        double sum2_ {0.0f};
        double min_ {std::numeric_limits<double>::max()};
        double max_ {-std::numeric_limits<double>::max()};

    public:
        void add_value(double value) {
            count_++;
            sum_ += value;
            sum2_ += value*value;
            min_ = std::min(min_, value);
            max_ = std::max(max_, value);
        }
        int count() const { return count_; }
        double mean() const { return sum_/count_; }
        double std_dev() const { return std::sqrt(sum2_/count_ - mean()*mean()); }
        double min() const { return min_; }
        double max() const { return max_; }
        stats_t stats() const {
            return {mean(), std_dev(), min(), max()};
        }
};

#endif
