
if(ESP_PLATFORM)
    idf_component_register( SRCS "main.cpp"
                            REQUIRES driver
                            INCLUDE_DIRS "." "include" $ENV{CADMIUM})

    target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++2b)
    target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
    target_compile_options(${COMPONENT_LIB} PRIVATE "-frtti")
    target_compile_options(${COMPONENT_LIB} PRIVATE "-fexceptions")
    # target_compile_options(${COMPONENT_LIB} PRIVATE "-DNO_LOGGING")
    # target_compile_options(${COMPONENT_LIB} PRIVATE "-DNO_LOG_STATE")
    # target_compile_options(${COMPONENT_LIB} PRIVATE "-DDEBUG_DELAY")
else()

    # Regular CMake project setup for non-ESP32
    add_executable(${projectName} main.cpp)
    add_executable(player_tester include/test/player/main_player_tester.cpp)
    add_executable(game_score_tester include/test/game_score/main_game_score_tester.cpp)
    add_executable(substitution_logic_tester include/test/substitution_logic/main_substitution_logic_tester.cpp)

    # Add required libraries and include directories
    target_include_directories(${projectName} PRIVATE "." "include" $ENV{CADMIUM})
    target_include_directories(player_tester PRIVATE "." "include" $ENV{CADMIUM})
    target_include_directories(game_score_tester PRIVATE "." "include" $ENV{CADMIUM})
    target_include_directories(substitution_logic_tester PRIVATE "." "include" $ENV{CADMIUM})

    # Link utils.cpp file for compilation
    target_sources(${projectName} PRIVATE include/data_structures/utils.cpp)
    target_sources(player_tester PRIVATE include/data_structures/utils.cpp)
    target_sources(game_score_tester PRIVATE include/data_structures/utils.cpp)
    target_sources(substitution_logic_tester PRIVATE include/data_structures/utils.cpp)

    # Non-ESP32 specific compile options
    target_compile_options(${projectName} PUBLIC -std=gnu++2b)
    target_compile_options(player_tester PUBLIC -std=gnu++2b)
    target_compile_options(game_score_tester PUBLIC -std=gnu++2b)
    target_compile_options(substitution_logic_tester PUBLIC -std=gnu++2b)
endif()