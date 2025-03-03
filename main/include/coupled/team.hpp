#ifndef TEAM_HPP
#define TEAM_HPP

#include "cadmium/modeling/devs/coupled.hpp" // cadmium header
#include "cadmium/lib/iestream.hpp"          // iestream header
#include "atomic/player.hpp"                 // player atomic model file

#include "data_structures/utils.hpp"

using namespace cadmium;

struct team: public Coupled {
    // Port definition for inputs
    Port<substitutionCommand> substitution_in;
    Port<GameState> game_state_in;

    // Port definition for outputs
    Port<playerMetrics> player_1_out;
    Port<playerMetrics> player_2_out;

    team(const std::string& id): Coupled(id) {
        // input port
        substitution_in = addInPort<substitutionCommand>("substitution_in");
        game_state_in = addInPort<GameState>("game_state_in");

        // output port
        player_1_out = addOutPort<playerMetrics>("player_1_out");
        player_2_out = addOutPort<playerMetrics>("player_2_out");

        // create player models
        auto player_1_model = addComponent<player>("player_1_model", 1, Phase::ON_FIELD);
        auto player_2_model = addComponent<player>("player_2_model", 2, Phase::ON_BENCH);

        // internal couplings
        addCoupling(substitution_in, player_1_model->substitution_in);
        addCoupling(substitution_in, player_2_model->substitution_in);
        addCoupling(game_state_in, player_1_model->game_state_in);
        addCoupling(game_state_in, player_2_model->game_state_in);

        addCoupling(player_1_model->player_out, player_1_out);
        addCoupling(player_2_model->player_out, player_2_out);
    }
};

#endif