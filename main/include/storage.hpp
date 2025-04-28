//===================== storage.hpp =====================
#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <iostream>
#include <limits>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct StorageState {
    double current_demand;  // kW
    double stored_energy;  // kWh
    double grid_balance;   // 0.0-1.0
    double sigma;
    
    explicit StorageState() 
     : stored_energy(500.0), grid_balance(1.0),current_demand(0),sigma(0.0){}
};

std::ostream& operator<<(std::ostream& os, const StorageState& state) {
    os << "{stored:" << state.stored_energy << "kWh, balance:" << state.grid_balance  <<",demand:"<< state.current_demand<< "KW}"; 
    return os;
}

class StorageModel : public Atomic<StorageState> {
public:
    Port<double> inPower;     
    Port<double> inDemand;    
    Port<double> outStatus;   

    StorageModel(const std::string& id) : Atomic<StorageState>(id, StorageState()) {
        inPower = addInPort<double>("inPower");
        inDemand = addInPort<double>("inDemand");
        outStatus = addOutPort<double>("outStatus");
    }
    void internalTransition(StorageState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
        
    }

    void externalTransition(StorageState& state, double e) const override {
        
        const double max_storage = 1000.0;
        const double min_storage = 0.0;
        double old_energy = state.stored_energy;
        
        if (!inDemand->empty()) {
            double new_demand = inDemand->getBag().back();
            
            if (new_demand <= 0.0) new_demand = 0;
            
            state.current_demand = new_demand; 
            state.sigma=0.0;
        }
    
        
        if (!inPower->empty()) {
            double total_power = 0.0;
            
            
            for (const auto& msg : inPower->getBag()) {
                
                total_power += msg;
            }
           
            
            double net_power = total_power - state.current_demand; 
            double delta = net_power;
            
            double new_energy = state.stored_energy + delta;
             
          
            state.stored_energy = std::clamp(new_energy, min_storage, max_storage);
            state.sigma = 0.0;
        }
    
        
        state.grid_balance = state.stored_energy / max_storage;
        state.grid_balance = std::clamp(state.grid_balance, 0.0, 1.0);
    }

    void output(const StorageState& state) const override {
        
        outStatus->addMessage(state.grid_balance);
    }

    [[nodiscard]] double timeAdvance(const StorageState& state) const override {
        return state.sigma;  
    }
};
#endif