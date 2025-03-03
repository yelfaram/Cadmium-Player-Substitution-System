# DEVS - Player Substitution System (PSS)

## Introduction
This project implements a DEVS-based model for a player substitution system. The model evaluates player metrics and determines when a substitution should occur based on the game state, player fatigue, mental state, and time played.

## Dependencies
This project assumes that you have **Cadmium** installed in a location accessible by the environment variable $CADMIUM.
_This dependency would be met by default if you are using the ARSLAB servers. To check, try `echo $CADMIUM` in the terminal_

## Build
To build this project, run:
```sh
source build_sim.sh
```
__NOTE__: Every time you run `build_sim.sh`, the contents of `build/` and `bin/` will be replaced and the contents of `simultation_results` will be removed.

## Changing File Paths
Before executing and testing the system, please ensure that the absolute paths for the following files are updated correctly in the respective header files:
- **main/include/test/game_score/game_score_tester.hpp** – Input files are located in `input_data/game_score/`
- **main/include/test/player/player_tester.hpp** – Input files are located in `input_data/player/`
- **main/include/test/substitution_logic/substitution_logic.hpp** – Input files are located in `input_data/substitution_logic/`
- **main/include/top/coach.hpp** – Input files are located in `input_data/top/`
__NOTE__: This change is needed to ensure **IEStream** reads the correct file from the right location.

### How to Update File Paths
Please replace the placeholder `"<absolute_path_here>"` with the correct absolute path for each of the above files in your local directory using the following method in Visual Studio Code:
1. Right-click the *.txt* file.
2. Select **Copy Path**.
3. Paste the copied path into the appropriate section of the code.

## Execute
To run the project, run:
```sh
./bin/player_substitution_system
```

## Testing Atomic Models
To test individual components of the system (such as substitution logic and player evaluation), navigate to the `bin` directory and run:
```sh
cd bin
./substitution_logic_tester
./player_tester
./game_score_tester
```

## Output Files
All output generated during the simulation will be saved as CSV files within the `simulation_results` folder located at the topmost level of the project directory. These files contain the results of each test and are named according to the respective tester (e.g., `game_score_output.csv`, `player_output.csv`, etc.).
