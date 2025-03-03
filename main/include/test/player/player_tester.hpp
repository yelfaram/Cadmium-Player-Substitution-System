#ifndef PLAYER_TESTER_HPP
#define PLAYER_TESTER_HPP

#include "cadmium/modeling/devs/coupled.hpp" // cadmium header
#include "cadmium/lib/iestream.hpp"          // iestream header
#include "atomic/player.hpp"                 // player atomic model file

#include "data_structures/utils.hpp"

using namespace cadmium;

struct player_tester: public Coupled {

    player_tester(const std::string& id): Coupled(id){
        auto player_model = addComponent<player>("player_model", 1, Phase::ON_FIELD);
        auto substitution_file = addComponent<lib::IEStream<substitutionCommand>>("substitution_file", "<absolute_path_here>");
        auto game_state_file = addComponent<lib::IEStream<GameState>>("game_state_file", "<absolute_path_here>");

        // internal couplings
        addCoupling(substitution_file->out, player_model->substitution_in);
        addCoupling(game_state_file->out, player_model->game_state_in);
    }
};

#endif