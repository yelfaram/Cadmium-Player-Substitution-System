#include <limits> //Required for infinity
#include "include/test/substitution_logic/substitution_logic_tester.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
    auto model = std::make_shared<substitution_logic_tester> ("substitution logic tester");
    auto rootCoordinator = RootCoordinator(model);

    // rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.setLogger<CSVLogger>("simulation_results/substitution_logic_output.csv", ";");


    rootCoordinator.start();
	rootCoordinator.simulate(60.0);
	rootCoordinator.stop();	

    return 0;
}