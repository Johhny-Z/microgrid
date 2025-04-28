#ifndef WIND_HPP
#define WIND_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp" //Cadmium header

using namespace cadmium; //This tells the compiler we are dealing with Cadmium


#include <cadmium/modeling/devs/atomic.hpp>

struct WindState {
    double power;
    double sigma;
    explicit WindState() : power(0.0),sigma(0.0){}
};

std::ostream& operator<<(std::ostream& os, const WindState& state) {
    os << "Power: " << state.power;
    return os;
}

class WindModel : public cadmium::Atomic<WindState> {
public:
    cadmium::Port<double> wind_speed_in;   
    cadmium::Port<double> power_out;       

    WindModel(const std::string& id) : Atomic<WindState>(id, WindState()) {
        wind_speed_in = addInPort<double>("wind_speed_in");
        power_out = addOutPort<double>("power_out");
    }

    void internalTransition(WindState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
        
    }

    void externalTransition(WindState& state, double e) const override {
        if (!wind_speed_in->empty()) {
            double speed = wind_speed_in->getBag().back();
            state.power = 0.5 * 1.225 * 3.14 * speed * speed * speed; 
            state.sigma = 0.0; 
        }
    }

    void output(const WindState& state) const override {
        power_out->addMessage(state.power);
        
    }

    [[nodiscard]] double timeAdvance(const WindState& state) const override {
        return  state.sigma;
    }
};

#endif