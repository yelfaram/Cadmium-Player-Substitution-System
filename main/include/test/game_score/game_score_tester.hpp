#ifndef GAME_SCORE_TESTER_HPP
#define GAME_SCORE_TESTER_HPP

#include "cadmium/modeling/devs/coupled.hpp" // cadmium header
#include "cadmium/lib/iestream.hpp"          // iestream header
#include "atomic/game_score.hpp"             // game score atomic model file

#include "data_structures/utils.hpp"

using namespace cadmium;

struct game_score_tester: public Coupled {

    game_score_tester(const std::string& id): Coupled(id){
        auto game_score_model = addComponent<gameScore>("game_score_model");
        auto team_1_score_file = addComponent<lib::IEStream<int>>("team_1_score_file", "<absolute_path_here>");
        auto team_2_score_file = addComponent<lib::IEStream<int>>("team_2_score_file", "<absolute_path_here>");

        // internal couplings
        addCoupling(team_1_score_file->out, game_score_model->team_1_score_in);
        addCoupling(team_2_score_file->out, game_score_model->team_2_score_in);
    }
};

#endif