#include <iostream>
#include <set>
#include "game.h"

game_t::game_t(const player_t &first, const player_t &second) :
        field() {
    players.push_back(first);
    players[0]->player_num = 0;
    players[0]->checker_amount = 12;
    players.push_back(second);
    players[1]->player_num = 1;
    players[1]->checker_amount = 12;
}

apply_step_t
game_t::apply_step(const step_t &step, size_t player_num, std::set<std::pair<int, int> > &attack_checkers) {
    char &curr_cell = field.fld[step.y_from - 1][step.x_from - 1];
    char &destination_cell = field.fld[step.y_to - 1][step.x_to - 1];
    bool is_attack = players[player_num]->possible_attack(field, attack_checkers);
    size_t next_player = (player_num + 1) % 2;

    int napr  = -1;
    int queen = 8;
    if(player_num == 1){
        napr = 1;
        queen = 1;
    }

    if ((destination_cell != '+') || (curr_cell == '-') || (curr_cell == '+')) {
        return {false, player_num};
    }

        if (curr_cell == (char) (player_num + 97) && (step.y_from - step.y_to) == napr && abs(step.x_to - step.x_from) == 1 && step.y_to == queen &&
            !is_attack) {
            std::swap(destination_cell, curr_cell);
            destination_cell = (char) (player_num + 65);
            return {true, next_player};
        }
        if (curr_cell == (char) (player_num + 97) && (step.y_from - step.y_to) == napr && abs(step.x_from - step.x_to) == 1 && !is_attack) {
            std::swap(destination_cell, curr_cell);
            return {true, next_player};
        }
        if (curr_cell == (char) (player_num + 65) && abs(step.y_to - step.y_from) == abs(step.x_to - step.x_from) && !is_attack) {
            bool empty_road = true;
            int k_y = (step.y_to - step.y_from) / abs(step.y_to - step.y_from);
            int k_x = (step.x_to - step.x_from) / abs(step.x_to - step.x_from);
            for (int i = 1; i < abs(step.y_to - step.y_from); ++i) {
                if (field.fld[step.y_from + i * k_y - 1][step.x_from + i * k_x - 1] != '+') {
                    empty_road = false;
                }
            }

            if (empty_road) {
                std::swap(destination_cell, curr_cell);
                return {true, next_player};
            }
        }
        if (is_attack && attack_checkers.count(std::pair<size_t, size_t>(step.x_from - 1, step.y_from - 1))) {
            bool next_attack;
            step_t next_step = step;

            do {
                bool is_correct_step = false;
                while (!is_correct_step) {
                    if (abs(next_step.y_from - next_step.y_to) == 2 && abs(next_step.x_from - next_step.x_to) == 2 &&
                        field.fld[next_step.y_to - 1][next_step.x_to - 1] == '+') {
                        size_t min_y = std::min(next_step.y_to, next_step.y_from);
                        size_t min_x = std::min(next_step.x_to, next_step.x_from);
                        char &middle_cell = field.fld[min_y][min_x];
                        if (middle_cell != (char) (player_num + 97) && middle_cell != (char) (player_num + 65) &&
                            middle_cell != '+') {
                            --players[next_player]->checker_amount;
                            middle_cell = '+';
                            std::swap(field.fld[next_step.y_from - 1][next_step.x_from - 1],
                                      field.fld[next_step.y_to - 1][next_step.x_to - 1]);
                            if (next_step.y_to == 1) {
                                field.fld[next_step.y_to - 1][next_step.x_to - 1] = (char) (player_num + 65);
                                return {true, next_player};
                            }
                            is_correct_step = true;
                        }
                    } else if (abs(next_step.y_from - next_step.y_to) == abs(next_step.x_from - next_step.x_to) &&
                               abs(next_step.x_from - next_step.x_to) > 2 &&
                               field.fld[next_step.y_to - 1][next_step.x_to - 1] == '+') {
                        bool enemy = false;
                        std::pair<size_t, size_t> enemy_pos;
                        int k_y = (next_step.y_to - next_step.y_from) / abs(next_step.y_to - next_step.y_from);
                        int k_x = (next_step.x_to - next_step.x_from) / abs(next_step.x_to - next_step.x_from);
                        for (int i = 1; i < abs(next_step.y_to - next_step.y_from); ++i) {
                            if (enemy &&
                                field.fld[next_step.y_from + i * k_y - 1][next_step.x_from + i * k_x - 1] != '+') {
                                enemy = false;
                                break;
                            }
                            if (field.fld[next_step.y_from + i * k_y - 1][next_step.x_from + i * k_x - 1] != (char) (player_num + 97) && field.fld[next_step.y_from + i * k_y - 1][next_step.x_from + i * k_x - 1] != (char) (player_num + 65) && field.fld[next_step.y_from + i * k_y - 1][next_step.x_from + i * k_x - 1] != '+') {
                                enemy = true;
                                enemy_pos.first = next_step.x_from + i * k_x - 1;
                                enemy_pos.second = next_step.y_from + i * k_y - 1;
                            }
                        }
                        if (enemy) {
                            --players[next_player]->checker_amount;
                            std::swap(field.fld[next_step.y_from - 1][next_step.x_from - 1],
                                      field.fld[next_step.y_to - 1][next_step.x_to - 1]);
                            if (next_step.y_to == 1 && field.fld[next_step.y_to - 1][next_step.x_to - 1] != (char) (player_num + 65)) {
                                field.fld[next_step.y_to - 1][next_step.x_to - 1] = (char) (player_num + 65);
                                return {true, next_player};
                            }
                            field.fld[enemy_pos.second][enemy_pos.first] = '+';
                            is_correct_step = true;
                        }

                    }

                    if (!is_correct_step) {
                        players[player_num]->on_incorrect_step(next_step);
                        next_step = players[player_num]->next_step(field,
                                                                   std::pair<size_t, size_t>(next_step.x_from,
                                                                                             next_step.y_from));
                    }
                }
                next_attack = players[player_num]->attacked_checkers(field, attack_checkers,
                                                                     std::pair<size_t, size_t>(next_step.x_to - 1,
                                                                                               next_step.y_to - 1));
                if (next_attack) {
                    next_step = players[player_num]->next_step(field,
                                                               std::pair<size_t, size_t>(next_step.x_to,
                                                                                         next_step.y_to));
                }
            } while (next_attack);
            return {true, next_player};
        }


    return {false, player_num};
}

game_t::game_outcome_t game_t::is_win(size_t player_num) const {
    std::set<std::pair<int, int>> attack_checkers;

    player_num = (player_num + 1) % 2;
    bool can_move = false;
    int a = 1;
    if (player_num == 1){
        a = -1;
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
                if (field.fld[row][col] == (char) (player_num + 97) &&
                    (field.fld[row + a][col + 1] == '+' || field.fld[row + a][col - 1] == '+')) {
                    can_move = true;
                }
                if (field.fld[row][col] == (char) (player_num + 65) || field.fld[row][col] == (char) (player_num + 97)) {
                    int change_step[2] = {-1, 1};
                    for (auto &i: change_step) {
                        for (auto &j: change_step) {
                            if (field.fld[row + i][col + j] == '+' && field.fld[row][col] == (char) (player_num + 97)) {
                                can_move = true;
                                break;
                            }
                            if ((field.fld[row + i][col + j] != (char) (player_num + 97) && field.fld[row + i][col + j] != (char) (player_num + 65) && field.fld[row + i][col + j] != '+') &&
                                field.fld[row + i * 2][col + j * 2] == '+') {
                                can_move = true;
                                break;
                            }
                        }
                    }
                }

        }
    }

    if (!can_move) {
        return
                WIN;
    }

    if (steps_amount > 1000) {
        return
                TIE;
    }

    if (players[0]->checker_amount == 0 || players[1]->checker_amount == 0) {
        return
                WIN;

    }
    return
            IN_PROGRESS;
}

void game_t::play() {
    std::set<std::pair<int, int>> attack_checkers;
    apply_step_t apply_result(false, 0);
    size_t counter = apply_result.player_num;
    while (is_win(counter) == IN_PROGRESS) {
        ++steps_amount;
        counter = apply_result.player_num;
        bool is_correct = false;
        while (!is_correct) {
            step_t step = players[counter]->make_step(field);
            apply_result = apply_step(step, counter, attack_checkers);
            is_correct = apply_result.is_correct;
            if (!is_correct) {
                players[counter]->on_incorrect_step(step);
            }
        }
    }

    if (is_win(counter) == TIE) {
        for (const auto &p : players) {
            p->on_tie();
        }
        return;
    }

    for (size_t i = 0; i < players.size(); ++i) {
        if (i == counter) {
            players[i]->on_win();
        } else {
            players[i]->on_lose();
        }
    }
}
