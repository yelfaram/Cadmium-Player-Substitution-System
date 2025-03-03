#ifndef SUBSTITUTION_LOGIC_TESTER_HPP
#define SUBSTITUTION_LOGIC_TESTER_HPP

#include "cadmium/modeling/devs/coupled.hpp" // cadmium header
#include "cadmium/lib/iestream.hpp"          // iestream header
#include "atomic/substitution_logic.hpp"     // substitution logic atomic model file

#include "data_structures/utils.hpp"

using namespace cadmium;

struct substitution_logic_tester: public Coupled {

    substitution_logic_tester(const std::string& id): Coupled(id){
        auto substitution_logic_model = addComponent<substitutionLogic>("substitution_logic_model");
        auto player_1_file = addComponent<lib::IEStream<playerMetrics>>("player_1_file", "<absolute_path_here>");
        auto player_2_file = addComponent<lib::IEStream<playerMetrics>>("player_2_file", "<absolute_path_here>");
        auto game_state_file = addComponent<lib::IEStream<GameState>>("game_state_file", "<absolute_path_here>");

        // internal couplings
        addCoupling(player_1_file->out, substitution_logic_model->player_1_in);
        addCoupling(player_2_file->out, substitution_logic_model->player_2_in);
        addCoupling(game_state_file->out, substitution_logic_model->game_state_in);
    }
};

#endif