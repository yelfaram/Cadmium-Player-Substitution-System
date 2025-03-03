#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>

using namespace std;

enum Phase {
    ON_FIELD,
    ON_BENCH
};

enum GameState {
    WINNING,
    LOSING,
    TIED
};

struct substitutionCommand {
    int player_out_id;      // id of player getting subbed out
    int player_in_id;       // id of player getting subbed in

    explicit substitutionCommand(): player_out_id(0), player_in_id(0){}
};

struct testOutput {
    int player_out_id;      // id of player getting subbed out
    int player_in_id;

    double score;
    double threshold;

    explicit testOutput(): player_out_id(0), player_in_id(0), score(0.0), threshold(0.0){}
};

struct playerMetrics {
    int player_id;
    double mental;
    double fatigue;
    Phase phase;
    double time_played;
    double time_on_bench;

    explicit playerMetrics(): player_id(0), mental(100.0), fatigue(0.0), phase(Phase::ON_BENCH), time_played(0.0), time_on_bench(0.0){}
};

struct metricWeights {
    double fatigue;
    double mental;
    double time_played;

    explicit metricWeights(): fatigue(0.0), mental(100.0), time_played(0.0){}

    metricWeights(double f, double m, double t) : fatigue(f), mental(m), time_played(t){}
};

istream& operator>>(istream& is, substitutionCommand& sub_cmd);
istream& operator>>(istream& is, GameState& game_state);
istream& operator>>(istream& is, Phase& phase);
istream& operator>>(istream& is, playerMetrics& metrics);

ostream& operator<<(ostream& out, const substitutionCommand& sub_cmd);
ostream& operator<<(ostream& out, const GameState& game_state);
ostream& operator<<(ostream& out, const Phase& phase);
ostream& operator<<(ostream& out, const playerMetrics& metrics);

#endif