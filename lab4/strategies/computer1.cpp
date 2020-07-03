#include "computer1.h"

#include <cassert>
#include <iostream>
#include <random>
#include <vector>


computer_strategy_t_1::computer_strategy_t_1(std::string name) :
        name(std::move(name)) {}

step_t computer_strategy_t_1::make_step(const field_t &fld) {
    std::set<std::pair<int, int>> attack_checkers;
    std::set<std::pair<int, int>> move_checkers;
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    int dest = 1;
    if (player_num == -1){
        dest = -1;
    }
        bool changed;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (fld.fld[row][col] == (char) (player_num + 97) || fld.fld[row][col] == (char) (player_num + 65)) {
                    changed = attacked_checkers(fld, attack_checkers, std::pair<size_t, size_t>(col, row));

                    if (!changed) {
                        if ((fld.fld[row + dest][col + 1] == '+' || fld.fld[row + dest][col - 1] == '+') &&
                            fld.fld[row][col] == (char) (player_num + 97)) {
                            move_checkers.insert(std::pair<size_t, size_t>(col, row));
                        }
                        if ((fld.fld[row + 1][col + 1] == '+' || fld.fld[row + 1][col - 1] == '+' ||
                             fld.fld[row - 1][col + 1] == '+' || fld.fld[row - 1][col - 1] == '+') &&
                            fld.fld[row][col] == (char) (player_num + 65)) {
                            move_checkers.insert(std::pair<size_t, size_t>(col, row));
                        }
                    }
                }
                changed = false;
            }
        }
        std::vector<std::pair<int, int>> possible_moves;


        if (!attack_checkers.empty()) {
            std::vector<std::pair<int, int>> possible_checkers(attack_checkers.begin(), attack_checkers.end());
            std::shuffle(possible_checkers.begin(), possible_checkers.end(), std::mt19937(std::random_device()()));
            selected_checker = possible_checkers.front();

            if (fld.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 65) ||
                fld.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 97)) {
                int max_R;
                if (fld.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 65)) {
                    max_R = std::max(selected_checker.second - 0, 7 - selected_checker.second);
                } else {
                    max_R = 2;
                }
                int change_step[2] = {-1, 1};
                for (auto &i: change_step) {
                    for (auto &j: change_step) {
                        bool enemy = false;
                        for (int R = 1; R <= max_R; ++R) {
                            if (enemy &&
                                fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] != '+') {
                                break;
                            }
                            if (fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] != (char) (player_num + 97) && fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] != (char) (player_num + 65) && fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] != '+') {
                                enemy = true;
                            }
                            if (enemy &&
                                fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] == '+') {
                                possible_moves.emplace_back(selected_checker.first + j * R,
                                                            selected_checker.second + i * R);
                            }
                        }
                    }
                }
            }

            std::shuffle(possible_moves.begin(), possible_moves.end(), std::mt19937(std::random_device()()));
            selected_target = possible_moves.front();
        } else if (!move_checkers.empty()) {
            std::vector<std::pair<int, int>> possible_checkers(move_checkers.begin(), move_checkers.end());
            std::shuffle(possible_checkers.begin(), possible_checkers.end(), std::mt19937(std::random_device()()));
            selected_checker = possible_checkers.front();

            if (fld.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 97)) {
                if (fld.fld[selected_checker.second + dest][selected_checker.first + 1] == '+') {
                    possible_moves.emplace_back(selected_checker.first + 1, selected_checker.second + dest);
                } else {
                    possible_moves.emplace_back(selected_checker.first - 1, selected_checker.second + dest);
                }
            }

            if (fld.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 65)) {
                size_t max_R = std::max(selected_checker.second - 0, 7 - selected_checker.second);
                int change_step[2] = {-1, 1};
                for (auto &i: change_step) {
                    for (auto &j: change_step) {
                        for (int R = 1; R < max_R; ++R) {
                            if (fld.fld[selected_checker.second + i * R][selected_checker.first + j * R] == '+') {
                                possible_moves.emplace_back(selected_checker.first + j * R,
                                                            selected_checker.second + i * R);
                            } else {
                                break;
                            }
                        }
                    }
                }
            }

            std::shuffle(possible_moves.begin(), possible_moves.end(), std::mt19937(std::random_device()()));
            selected_target = possible_moves.front();
        }


    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}

bool computer_strategy_t_1::possible_attack(const field_t &fld, std::set<std::pair<int, int> > &attack_checker) {
    attack_checker.clear();
    bool changed = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

                if (fld.fld[i][j] == (char) (player_num + 97) || fld.fld[i][j] == (char) (player_num + 65)) {
                    if (attacked_checkers(fld, attack_checker, std::pair<size_t, size_t>(j, i))) {
                        changed = true;
                    }
                }

        }
    }
    return changed;
}

step_t computer_strategy_t_1::next_step(const field_t &field, std::pair<size_t, size_t> current_checker) {
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    std::vector<std::pair<int, int>> possible_moves;

    selected_checker.first = current_checker.first - 1;
    selected_checker.second = current_checker.second - 1;


        if (field.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 97)) {
            int change_step[2] = {-1, 1};
            for (auto &i: change_step) {
                for (auto &j: change_step) {
                    if (field.fld[selected_checker.second + i][selected_checker.first + j] == (char) (player_num + 97) ||
                        field.fld[selected_checker.second + i][selected_checker.first + j] == (char) (player_num + 65)) {
                        if (field.fld[selected_checker.second + i * 2][selected_checker.first + j * 2] == '+') {
                            possible_moves.emplace_back(selected_checker.first + j * 2,
                                                        selected_checker.second + i * 2);

                        }
                    }
                }
            }
        }

        if (field.fld[selected_checker.second][selected_checker.first] == (char) (player_num + 65)) {
            size_t max_R = std::max(selected_checker.second - 0, 7 - selected_checker.second);
            int change_step[2] = {-1, 1};
            for (auto &i: change_step) {
                for (auto &j: change_step) {
                    bool enemy = false;
                    for (int R = 1; R <= max_R; ++R) {
                        if (enemy &&
                            field.fld[selected_checker.second + i * R][selected_checker.first + j * R] != '+') {
                            break;
                        }
                        if (field.fld[selected_checker.second + i * R][selected_checker.first + j * R] != (char) (player_num + 97) && field.fld[selected_checker.second + i * R][selected_checker.first + j * R] != (char) (player_num + 65) && field.fld[selected_checker.second + i * R][selected_checker.first + j * R] != '+') {
                            enemy = true;
                        }
                        if (enemy &&
                            field.fld[selected_checker.second + i * R][selected_checker.first + j * R] == '+') {
                            possible_moves.emplace_back(selected_checker.first + j * R,
                                                        selected_checker.second + i * R);
                        }
                    }
                }
            }
        }


    std::shuffle(possible_moves.begin(), possible_moves.end(), std::mt19937(std::random_device()()));
    selected_target = possible_moves.front();

    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}

void computer_strategy_t_1::print_stat() const {
    std::cout << "Random model [" << name << "]: " << std::endl;
    computer_interface_t::print_stat();
}
