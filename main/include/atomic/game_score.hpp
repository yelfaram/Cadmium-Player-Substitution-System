#ifndef GAME_SCORE_HPP
#define GAME_SCORE_HPP

#include <random>
#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

#include "data_structures/utils.hpp"

using namespace cadmium;

struct gameScoreState {
    int team_1_score;      // tracks score of team 1
    int team_2_score;      // tracks score of team 2
    GameState game_state;        // tracks current game state
    double sigma;          // Time tgo the next internal event; initial 1

    explicit gameScoreState(): team_1_score(0), team_2_score(0), game_state(GameState::TIED), sigma(1.0){
    }
};

std::ostream& operator<<(std::ostream &out, const gameScoreState& state) {
    out  << "{game state: " << state.game_state << ", team 1 score: " << state.team_1_score << ", team 2 score: " << state.team_2_score << "}"; 
    return out;
}

class gameScore: public Atomic<gameScoreState> {
    public:
    Port<int> team_1_score_in;  // input score of team 1
    Port<int> team_2_score_in;  // input score of team 2
    Port<GameState> game_state_out;   // outputs the game state (favors team 1)

    gameScore(const std::string id) : Atomic<gameScoreState>(id, gameScoreState()) {
        //Constructor of your atomic model. Initialize ports here.
        team_1_score_in = addInPort<int>("team_1_score_in");
        team_2_score_in = addInPort<int>("team_2_score_in");
        game_state_out = addOutPort<GameState>("game_state_out");
    }

    // inernal transition
    // Every ta(s) nothing happens apart from resetting sigma back to 1.0 (no internal events)
    void internalTransition(gameScoreState& state) const override {
        //your internal transition function goes here
        state.sigma = 1.0;
    }

    // external transition
    void externalTransition(gameScoreState& state, double e) const override {
        //your external transition function goes here
        if (!team_1_score_in->empty()) {
            state.team_1_score = team_1_score_in->getBag().back();
        }

        if (!team_2_score_in->empty()) {
            state.team_2_score = team_2_score_in->getBag().back();
        }

        if (state.team_1_score > state.team_2_score) {
            state.game_state = GameState::WINNING;
        }
        else if (state.team_1_score < state.team_2_score) {
            state.game_state = GameState::LOSING;
        } else {
            state.game_state = GameState::TIED;
        }

        state.sigma -= e;
    }

    // output function
    void output(const gameScoreState& state) const override {
        //your output function goes here
        game_state_out->addMessage(state.game_state);
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const gameScoreState& state) const override {     
            return state.sigma;
    }
};

#endif