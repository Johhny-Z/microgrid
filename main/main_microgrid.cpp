#include <limits>

#include "include/top_system.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;



int main() {
    auto model = std::make_shared<TopSystem>("microgrid");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<CSVLogger>("log.csv", ",");
    
    rootCoordinator.start();
	rootCoordinator.simulate(24.0);
	rootCoordinator.stop();
    
    return 0;
}