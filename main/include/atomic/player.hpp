#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <random>
#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

#include "data_structures/utils.hpp"

using namespace cadmium;

struct playerState {
    int player_id;              // unique identifier for player
    double mental;              // player mental level (100 to 0 - high confidence/low stress to low confidence/high stress)
    double fatigue;             // player fatigue level (0 to 100 - not tired to very tired)
    Phase phase;                // indicates whether player is on field or is on bench
    double time_played;         // tracks total time player has been on field
    double time_on_bench;       // tracks total time player has been on bench, recovering (resets to 0 on field)
    GameState game_state;       // tracks current game state
    double sigma;               // time to the next internal event; initial 1

    // Random number generation for mental event fluctuations (small mistake (25%), big mistake (10%), score a goal (5%), assist a goal (10%), no event (50%))
    // std::mt19937 rng;
    // std::discrete_distribution<int> event_dist;

    // rng(std::random_device{}() + (id * 35)),
    // event_dist{25, 10, 5, 10, 20},

    explicit playerState(int player_id, Phase phase): 
        player_id(player_id),
        mental(100.0),
        fatigue(0.0),
        phase(phase),
        time_played(0.0),
        time_on_bench(0.0),
        game_state(GameState::TIED),
        sigma(1.0){}
};

std::ostream& operator<<(std::ostream &out, const playerState& state) {
    out << "{player_id:" << state.player_id << ", game state: " << state.game_state << ", mental: " << state.mental << ", fatigue: " << state.fatigue << ", phase: " << state.phase << ", time played: " << state.time_played << ", time on bench: " << state.time_on_bench << "}";
    return out;
}

class player: public Atomic<playerState> {
    public:
    Port<substitutionCommand> substitution_in;  // input for player to substitute in and substitute out
    Port<GameState> game_state_in;              // input for current team status (winning, losing, or tied)
    Port<playerMetrics> player_out;             // outputs details of player
    

    player(const std::string& id, int player_id, Phase phase) : Atomic<playerState>(id, playerState(player_id, phase)) {
        //Constructor of your atomic model. Initialize ports here.
        player_out = addOutPort<playerMetrics>("player_out");   // set it to output port
        substitution_in = addInPort<substitutionCommand>("substitution_in"); // set it to input port
        game_state_in = addInPort<GameState>("game_state_in"); // set it to input port

    }

    // helper function for applying random mental fluctuations
    // void applyRandomMentalEvent(playerState& state) const {
    //     int event = state.event_dist(state.rng);
    //     switch (event) {
    //         case 0: state.mental -= 5;   break;      // small mistake (-5, 25%)
    //         case 1: state.mental -= 10;  break;      // big mistake (-10, 10%)
    //         case 2: state.mental += 15;  break;      // score a goal (+15, 5%)
    //         case 3: state.mental += 7.5; break;      // assist a goal (+7.5, 10%)
    //         case 4:                      break;      // no event (50%, no change)
    //     }
    // }

    // inernal transition
    // Every ta(s) we increment or decrement mental, fatigue, and time metrics
    // => rates are per minute for fatigue/mental/time (doesn't apply to rand mental event)
    // => fatigue | ON_FIELD (+1.5) | ON_BENCH (-1.0)
    // => mental  | ON_FIELD (+/- based on rand event & +/- based on game state) | ON_BENCH (+1.0)
    // => time    | ON_FIELD (+1.0  to time_played) | ON_BENCH (+1.0 to time_on_bench)
    // => mental and fatigue are bounded to use std::clamp [0, 100]
    void internalTransition(playerState& state) const override {
        //your internal transition function goes here
        if (state.phase == Phase::ON_FIELD) {
            // fatigue level
            state.fatigue = std::clamp(state.fatigue + (1.5 * state.sigma), 0.0, 100.0);

            // mental level => random fluctuations
            // applyRandomMentalEvent(state)

            // mental level => game state
            switch (state.game_state) {
                case GameState::WINNING: state.mental += (2.0 * state.sigma); break;
                case GameState::LOSING:  state.mental -= (2.0 * state.sigma); break;
                case GameState::TIED:                                         break;
            }

            state.mental = std::clamp(state.mental, 0.0, 100.0);

            // time metric => time played
            state.time_played += state.sigma;
        }
        else if (state.phase == Phase::ON_BENCH) {
            // fatigue level
            state.fatigue = std::clamp(state.fatigue - (1.0 * state.sigma), 0.0, 100.0);
            // mental level
            state.mental = std::clamp(state.mental + (1.0 * state.sigma), 0.0, 100.0);
            // time metric => time on bench
            state.time_on_bench += state.sigma;
        }
        
        // VERY CRUCIAL (forgot to do this)
        state.sigma = 1.0;      // reset sigma for next interval
    }

    // external transition
    void externalTransition(playerState& state, double e) const override {
        //your external transition function goes here
        // received id to substitute
        if (!substitution_in->empty()) {
            // substitute player id (receiving player id => toggle phase depending on out_id or in_id)
            substitutionCommand substitution_ids = substitution_in->getBag().back();

            // player will be subbed out
            if (substitution_ids.player_out_id == state.player_id && state.phase == Phase::ON_FIELD) {
                state.phase = Phase::ON_BENCH;          // toggle phase to bench
            }

            // player will be subbed in
            if (substitution_ids.player_in_id == state.player_id && state.phase == Phase::ON_BENCH) {
                state.phase = Phase::ON_FIELD;          // toggle phase to field
                state.time_on_bench = 0;   // reset time on bench
            }
        }

        // received new game score state
        if (!game_state_in->empty()) {
            state.game_state = game_state_in->getBag().back();
        }

        // update sigma function (since interrupted by event)
        state.sigma -= e;
    }
    
    // output function
    void output(const playerState& state) const override {
        //your output function goes here
        playerMetrics player_metrics;

        player_metrics.player_id = state.player_id;
        player_metrics.mental = state.mental;
        player_metrics.fatigue = state.fatigue;
        player_metrics.phase = state.phase;
        player_metrics.time_played = state.time_played;
        player_metrics.time_on_bench = state.time_on_bench;

        player_out->addMessage(player_metrics);
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const playerState& state) const override {     
            return state.sigma;
    }
};


#endif