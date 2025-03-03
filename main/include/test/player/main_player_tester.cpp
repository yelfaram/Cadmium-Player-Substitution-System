#include <limits> //Required for infinity
#include "include/test/player/player_tester.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
    auto model = std::make_shared<player_tester> ("player tester");
    auto rootCoordinator = RootCoordinator(model);

    // rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.setLogger<CSVLogger>("simulation_results/player_output.csv", ";");

    rootCoordinator.start();
	rootCoordinator.simulate(61.0);
	rootCoordinator.stop();	

    return 0;
}