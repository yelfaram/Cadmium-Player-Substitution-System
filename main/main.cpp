#include <limits>
#include "include/top/coah.hpp"

/*
There are 3 macros defined at compile time that changes the behaviour of the simulation.
--> SIM_TIME: This macro, when defined, runs the simulation in simulation time. Else, the simulation runs at wall clock.
--> ESP_PLATFORM: When defined, the models are compiled for the ESP32 microcontroller. Else, compiles for Linux/ Windows
--> NO_LOGGING: When defined, prevents logging (maybe useful in embedded situations)
*/


#ifdef SIM_TIME
	#include "cadmium/simulation/root_coordinator.hpp"
#else
	#include "cadmium/simulation/rt_root_coordinator.hpp"
	#ifdef ESP_PLATFORM
		#include <cadmium/simulation/rt_clock/ESPclock.hpp>
	#else
		#include <cadmium/simulation/rt_clock/chrono.hpp>
	#endif
#endif


#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

extern "C" {
	#ifdef ESP_PLATFORM
		void app_main() //starting point for ESP32 code
	#else
		int main()		//starting point for simulation code
	#endif
	{
	
		auto model = std::make_shared<coach> ("coach top");
		auto rootCoordinator = cadmium::RootCoordinator(model);

		// rootCoordinator.setLogger<STDOUTLogger>(";");
		rootCoordinator.setLogger<CSVLogger>("simulation_results/coach_output.csv", ";");

		rootCoordinator.start();
		rootCoordinator.simulate(100.0);
		rootCoordinator.stop();	

		return 0;

	}
}