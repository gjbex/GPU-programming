#ifndef ABSTRACT_MEASURE_HDR
#define ABSTRACT_MEASURE_HDR

#include "particles.h"

template <typename T>
struct Measure {
    protected:
        const std::string name_;
        const Particles& particles_;
        std::vector<T> values_;

    public:
        Measure(const std::string& name, const Particles& particles) :
            name_{name}, particles_{particles} {}
        std::string name() const { return name_; }
        virtual T compute_value() const = 0;
        void compute() {
            values_.push_back(compute_value());
        }
        virtual std::string current_value() const = 0;
        size_t size() const { return values_.size(); }
        T get_value(size_t i) const { return values_[i]; }
        ~Measure() = default;
};

struct MeasureBase {
    public:
        virtual std::string name() const = 0;
        virtual void compute() = 0;
        virtual std::string current_value() const = 0;
        ~MeasureBase() = default;
};

template <typename T>
struct MeasureWrapper : public MeasureBase {
    protected:
        Measure<T>* measure_;
    public:
        explicit MeasureWrapper(Measure<T>* measure) : measure_{measure} {}
        std::string name() const override { return measure_->name(); }
        void compute() override { measure_->compute(); }
        std::string current_value() const override { return measure_->current_value(); }
        size_t size() const { return measure_->size(); }
        T get_value(size_t i) const { return measure_->get_value(); }
        ~MeasureWrapper() {
            delete measure_;
        }
};

#endif
