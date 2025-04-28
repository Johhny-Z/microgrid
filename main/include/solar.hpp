#ifndef SOLAR_HPP
#define SOLAR_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp" //Cadmium header

using namespace cadmium; //This tells the compiler we are dealing with Cadmium


struct SolarState {
    double power;
    double sigma;
    explicit SolarState() : power(0.0),sigma(0.0) {}
};

std::ostream& operator<<(std::ostream& os, const SolarState& state) {
    os << "Power: " << state.power;
    return os;
}

class SolarModel : public cadmium::Atomic<SolarState> {
public:
    cadmium::Port<double> irradiance_in;   
    cadmium::Port<double> power_out;       

    SolarModel(const std::string& id) : Atomic<SolarState>(id, SolarState()) {
        irradiance_in = addInPort<double>("irradiance_in");
        power_out = addOutPort<double>("power_out");
    }

    void internalTransition(SolarState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
        
    }

    void externalTransition(SolarState& state, double e) const override {
        if (!irradiance_in->empty()) {
            double irradiance = irradiance_in->getBag().back();
            const double efficiency = 0.18;  
            state.power = irradiance * efficiency;
            state.sigma = 0.0;
        }
    }

    void output(const SolarState& state) const override {
        power_out->addMessage(state.power);
        
    }

    [[nodiscard]] double timeAdvance(const SolarState& state) const override {
        return  state.sigma;  
    }
};


#endif