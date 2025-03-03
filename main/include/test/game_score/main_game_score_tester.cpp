#include <limits> //Required for infinity
#include "include/test/game_score/game_score_tester.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;


int main() {
    auto model = std::make_shared<game_score_tester> ("game score tester");
    auto rootCoordinator = RootCoordinator(model);

    // rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.setLogger<CSVLogger>("simulation_results/game_score_output.csv", ";");

    rootCoordinator.start();
	rootCoordinator.simulate(5.0);
	rootCoordinator.stop();	

    return 0;
}