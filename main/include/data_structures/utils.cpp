#include <iostream>
#include <fstream>
#include <string>

#include "utils.hpp"

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>>(istream& is, substitutionCommand& sub_cmd) {
    is >> sub_cmd.player_out_id >> sub_cmd.player_in_id;
    return is;
}

istream& operator>>(istream& is, GameState& game_state) {
    int input;
    is >> input;
    
    switch (input) {
        case 0: game_state = GameState::WINNING; break;
        case 1: game_state = GameState::LOSING; break;
        case 2: game_state = GameState::TIED; break;
        default: game_state = GameState::TIED; break;
    }
    return is;
}

istream& operator>>(istream& is, Phase& phase) {
    int input;
    is >> input;
    
    switch (input) {
        case 0: phase = Phase::ON_FIELD; break;
        case 1: phase = Phase::ON_BENCH; break;
        default: phase = Phase::ON_BENCH; break;
    }
    return is;
}

istream& operator>>(istream& is, playerMetrics& metrics) {
    is >> metrics.player_id >> metrics.mental >> metrics.fatigue >> metrics.phase >> metrics.time_played >> metrics.time_on_bench;
    return is;
}

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream &out, const substitutionCommand& sub_cmd) {
    out << "{player_out_id: " << sub_cmd.player_out_id << ", player_in_id: " << sub_cmd.player_in_id << "}";
    return out;
}

ostream& operator<<(ostream &out, const GameState& game_state) {  
    switch (game_state) {  
        case GameState::WINNING: out << "WINNING"; break;  
        case GameState::LOSING:  out << "LOSING"; break;
        case GameState::TIED:    out << "TIED"; break;
        default: out << "ERROR"; break;
    }  
    return out;  
}

ostream& operator<<(ostream &out, const Phase& phase) {  
    switch (phase) {  
        case Phase::ON_FIELD: out << "ON_FIELD"; break;  
        case Phase::ON_BENCH: out << "ON_BENCH"; break;
        default: out << "ERROR"; break;  
    }  
    return out;  
} 

ostream& operator<<(ostream &out, const playerMetrics& metrics) {
    out << "{player_id:" << metrics.player_id << ", mental: " << metrics.mental << ", fatigue: " << metrics.fatigue << ", phase: " << metrics.phase << ", time played: " << metrics.time_played << ", time on bench: " << metrics.time_on_bench << "}";
    return out;
}