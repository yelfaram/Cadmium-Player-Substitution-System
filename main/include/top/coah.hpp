#ifndef COACH_HPP
#define COACH_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "atomic/game_score.hpp"             // game score atomic model file
#include "atomic/substitution_logic.hpp"     // substitution logic atomic model file
#include "coupled/team.hpp"                  // team coupled model file

using namespace cadmium;

struct coach : public Coupled {

    /**
     * Constructor function for the blinkySystem model.
     * @param id ID of the blinkySystem model.
     */
    coach(const std::string& id) : Coupled(id) {
        // create game score model which will be fed inputs of team scores
        auto game_score_model = addComponent<gameScore>("game_score_model");
        auto team_1_score_file = addComponent<lib::IEStream<int>>("team_1_score_file", "<absolute_path_here>");
        auto team_2_score_file = addComponent<lib::IEStream<int>>("team_2_score_file", "<absolute_path_here>");

        // internal couplings
        addCoupling(team_1_score_file->out, game_score_model->team_1_score_in);
        addCoupling(team_2_score_file->out, game_score_model->team_2_score_in);

        // create substitution logic model
        auto substitution_logic_model = addComponent<substitutionLogic>("substitution_logic_model");

        // create team coupled model
        auto team_model = addComponent<team>("team_coupled_model");

        // add coupling game score out -> substitution input
        addCoupling(game_score_model->game_state_out, substitution_logic_model->game_state_in);

        // add coupling game score out -> game state input
        addCoupling(game_score_model->game_state_out, team_model->game_state_in);

        // add coupling team out -> substitution input
        addCoupling(team_model->player_1_out, substitution_logic_model->player_1_in);
        addCoupling(team_model->player_2_out, substitution_logic_model->player_2_in);

        // add coupling substitution output -> team input
        addCoupling(substitution_logic_model->substitution_out, team_model->substitution_in);
    }
};

#endif