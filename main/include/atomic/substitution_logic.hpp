#ifndef SUBSTITUTION_LOGIC_HPP
#define SUBSTITUTION_LOGIC_HPP

#include <random>
#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

#include "data_structures/utils.hpp"

using namespace cadmium;

constexpr double SCALING_FACTOR = 45.0;   // time played scaling factor to help normalize it (depends on type of sport)
constexpr double MIN_REST = 10.0;         // minimum rest period threshold to be eligible for substitution
constexpr double MIN_MENTAL = 60;         // minimum mental threshold to be eligible for substitution
constexpr double MAX_FATIGUE = 40;        // maximum fatigue threshold to be eligible for substitution

struct normalizedMetrics {
    double mental_norm;
    double fatigue_norm;
    double time_played_norm;

    explicit normalizedMetrics(): mental_norm(0.0), fatigue_norm(0.0), time_played_norm(0.0){}

    normalizedMetrics(double m, double f, double t) : mental_norm(m), fatigue_norm(f), time_played_norm(t){}
};

struct substitutionLogicState {
    playerMetrics player_1;      // tracks player 1 metrics for evaluation
    playerMetrics player_2;      // tracks player 2 metrics for evaluation
    GameState game_state;        // tracks current game state
    double sigma;

    explicit substitutionLogicState(): game_state(GameState::TIED), sigma(1.0){}
};

std::ostream& operator<<(std::ostream &out, const substitutionLogicState& state) {
    out  << "{game state: " << state.game_state << ", player 1: " << state.player_1 << ", player 2: " << state.player_2 << "}";
    return out;
}

class substitutionLogic : public Atomic<substitutionLogicState> {
    public:
    Port<playerMetrics> player_1_in;
    Port<playerMetrics> player_2_in;
    Port<GameState> game_state_in;
    Port<substitutionCommand> substitution_out;

    substitutionLogic(const std::string id) : Atomic<substitutionLogicState>(id, substitutionLogicState()) {
        //Constructor of your atomic model. Initialize ports here.
        substitution_out = addOutPort<substitutionCommand>("substitution_out");
        player_1_in = addInPort<playerMetrics>("player_1_in");
        player_2_in = addInPort<playerMetrics>("player_2_in");
        game_state_in = addInPort<GameState>("game_state_in");
    }

    // inernal transition
    // Every ta(s) nothing happens apart from resetting sigma back to 1.0 (no internal events)
    void internalTransition(substitutionLogicState& state) const override {
        //your internal transition function goes here
        state.sigma = 1.0;
    }

    // external transition
    void externalTransition(substitutionLogicState& state, double e) const override {
        // your external transition function goes here

        // game state takes priority over players to make sure thresholds are up to date
        if (!game_state_in->empty()) {
            state.game_state = game_state_in->getBag().back();
        }
        
        // both use the same atomic file so will update at the same rate/sigma
        if (!player_1_in->empty()) {
            state.player_1 = player_1_in->getBag().back();
        }
        
        if(!player_2_in->empty()) {
            state.player_2 = player_2_in->getBag().back();
        }

        state.sigma -= e;
    }

    // helper functions
    metricWeights getWeights(GameState game_state) const {
        switch (game_state) {
            case GameState::WINNING: return metricWeights{1.2, 0.7, 1.0};
            case GameState::LOSING: return metricWeights{1.2, 1.5, 1.1};
            case GameState::TIED: return metricWeights{1.0, 1.0, 1.0};
            default: return metricWeights{1.0, 1.0, 1.0};
        }
    }

    double getThreshold(GameState game_state) const {
        switch (game_state) {
            case GameState::WINNING: return 1.8;
            case GameState::LOSING: return 1.2;
            case GameState::TIED: return 1.5;
            default: return 1.5;
        }
    }

    bool checkIfBenchPlayerReady(const playerMetrics& metrics) const {
        return (metrics.mental >= MIN_MENTAL && metrics.fatigue <= MAX_FATIGUE && metrics.time_on_bench >= MIN_REST);
    }

    normalizedMetrics normalizeMetrics(const playerMetrics& metrics) const {
        return {
            (100 - metrics.mental) / 100.0, 
            metrics.fatigue / 100.0, 
            std::tanh(metrics.time_played / SCALING_FACTOR)
        };
    }

    double getScore(const normalizedMetrics& norm_metrics, const metricWeights& weights) const {
        return (norm_metrics.fatigue_norm * weights.fatigue) + (norm_metrics.mental_norm * weights.mental) + (norm_metrics.time_played_norm * weights.time_played);
    }
    
    // output function
    void output(const substitutionLogicState& state) const override {
        //your output function goes here

        // make sure they both don't have same phase
        if (state.player_1.phase == state.player_2.phase) return;

        metricWeights weights = getWeights(state.game_state);
        double threshold = getThreshold(state.game_state);

        // for bench player we just evaluate against hardcoded values to check he meets substitution eligibility
        // for field player we use weighted decision making
        // => if bench player passes and field player passes threshold, then we substitute them
        const playerMetrics& bench_player = (state.player_1.phase == Phase::ON_BENCH) ? state.player_1 : state.player_2;
        const playerMetrics& field_player = (state.player_1.phase == Phase::ON_FIELD) ? state.player_1 : state.player_2;

        bool bench_ready = checkIfBenchPlayerReady(bench_player);
        normalizedMetrics norm_metrics = normalizeMetrics(field_player);
        double score = getScore(norm_metrics, weights);

        if (bench_ready && score > threshold) {
            substitutionCommand sub_cmd;
            sub_cmd.player_out_id = field_player.player_id;
            sub_cmd.player_in_id = bench_player.player_id;
            substitution_out->addMessage(sub_cmd);
        }
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const substitutionLogicState& state) const override {     
            return state.sigma;
    }
};

#endif