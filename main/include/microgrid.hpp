#ifndef MICROGRID_HPP
#define MICROGRID_HPP

#include "cadmium/modeling/devs/coupled.hpp" //cadmium header
#include "cadmium/lib/iestream.hpp"          //iestream header
#include "solar.hpp"
#include "wind.hpp"                       //counter atomic model file

using namespace cadmium;


struct Microgrid : public cadmium::Coupled {
    std::shared_ptr<SolarModel> solar;
    std::shared_ptr<WindModel> wind;
    Port<double> irradiance_in;  
    Port<double> wind_speed_in;
    Port<double> solar_power_out;  
    Port<double> wind_power_out;   
    
    Microgrid(const std::string& id) : Coupled(id) {
        
        solar = addComponent<SolarModel>("solar");
        wind = addComponent<WindModel>("wind");
       
        
        
        irradiance_in = addInPort<double>("irradiance_in");
        wind_speed_in = addInPort<double>("wind_speed_in");
        solar_power_out = addOutPort<double>("solar_power_out");  
        wind_power_out = addOutPort<double>("wind_power_out");     
        

        
        addCoupling(irradiance_in, solar->irradiance_in);
        addCoupling(wind_speed_in, wind->wind_speed_in);
        addCoupling(solar->power_out, solar_power_out);  
        addCoupling(wind->power_out, wind_power_out);    
        
        
        
    }
};
#endif