//===================== top_system.hpp =====================
#ifndef TOP_SYSTEM_HPP
#define TOP_SYSTEM_HPP
#include "cadmium/modeling/devs/coupled.hpp" //cadmium header
#include "cadmium/lib/iestream.hpp"          //iestream header
#include "microgrid.hpp"
#include "storage.hpp"

using namespace cadmium;

struct TopSystem : public Coupled {
    Port<double> outTotalPower;
    Port<double> outBattery;

    TopSystem(const std::string& id) : Coupled(id) {
        
        auto microgrid = addComponent<Microgrid>("microgrid");
        auto storage = addComponent<StorageModel>("storage");

        
        auto irradianceSrc = addComponent<lib::IEStream<double>>(
            "irradiance_src", "/home/jingqizhang/microgrid/inputs/irradiance.txt");
        auto windSpeedSrc = addComponent<lib::IEStream<double>>(
            "wind_speed_src", "/home/jingqizhang/microgrid/inputs/wind_speed.txt");
        auto demandSrc = addComponent<lib::IEStream<double>>(
            "demand_src", "/home/jingqizhang/microgrid/inputs/load_profile.txt");

        
        addCoupling(irradianceSrc->out, microgrid->irradiance_in);
        addCoupling(windSpeedSrc->out, microgrid->wind_speed_in);


        
        addCoupling(microgrid->solar_power_out, storage->inPower);
        
        addCoupling(microgrid->wind_power_out, storage->inPower);
        
        addCoupling(demandSrc->out, storage->inDemand);

        
        outTotalPower = addOutPort<double>("total_power");
        outBattery = addOutPort<double>("battery_status");
        addCoupling(microgrid->solar_power_out, outTotalPower);
        addCoupling(microgrid->wind_power_out, outTotalPower);
        addCoupling(storage->outStatus, outBattery);

    }
};
#endif