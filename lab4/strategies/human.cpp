#include "human.h"


human_strategy_t::human_strategy_t(std::string name) :
        name(std::move(name)) {}

void human_strategy_t::on_win() {
    std::cout << name << " - You win!" << std::endl;
}

void human_strategy_t::on_lose() {
    std::cout << name << " - You lose!" << std::endl;
}

void human_strategy_t::on_tie() {
    std::cout << "Tie!" << std::endl;
}

step_t human_strategy_t::make_step(const field_t &fld) {
    std::cout << "Field:" << std::endl;
    int i = 1;
    std::cout << "  A B C D E F G H" << std::endl;
    for (const auto &line : fld.fld) {
        std::cout << i << ' ';
        i++;
        for (char c : line) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "Turn of " << name << std::endl;
    std::cout << "Type coordinates of a checker: " << std::endl;
    int x_from = -1, y_from;
    char x_fr;
    std::cin >> x_fr >> y_from;
    if (x_fr == 'A') { x_from = 1;}
    if (x_fr == 'B') { x_from = 2;}
    if (x_fr == 'C') { x_from = 3;}
    if (x_fr == 'D') { x_from = 4;}
    if (x_fr == 'E') { x_from = 5;}
    if (x_fr == 'F') { x_from = 6;}
    if (x_fr == 'G') { x_from = 7;}
    if (x_fr == 'H') { x_from = 8;}

    std::cout << "Type coordinates of the move: " << std::endl;
    int x_to = -1, y_to;
    char x_t;
    std::cin >> x_t >> y_to;
    if (x_t == 'A') { x_to = 1;}
    if (x_t == 'B') { x_to = 2;}
    if (x_t == 'C') { x_to = 3;}
    if (x_t == 'D') { x_to = 4;}
    if (x_t == 'E') { x_to = 5;}
    if (x_t == 'F') { x_to = 6;}
    if (x_t == 'G') { x_to = 7;}
    if (x_t == 'H') { x_to = 8;}

    return {x_from, y_from, x_to, y_to};
}

step_t human_strategy_t::next_step(const field_t &fld, std::pair<size_t, size_t> current_checker) {
    std::cout << "Field:" << std::endl;
    int i = 1;
    std::cout << "  1 2 3 4 5 6 7 8" << std::endl;
    for (const auto &line : fld.fld) {
        std::cout << i << ' ';
        i++;
        for (char c : line) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    int x_from = current_checker.first;
    int y_from = current_checker.second;
    std::cout << "Type coordinates of the next move: " << std::endl;
    int x_to = -1, y_to;
    char x_t;
    std::cin >> x_t >> y_to;
    if (x_t == 'A') { x_to = 1;}
    if (x_t == 'B') { x_to = 2;}
    if (x_t == 'C') { x_to = 3;}
    if (x_t == 'D') { x_to = 4;}
    if (x_t == 'E') { x_to = 5;}
    if (x_t == 'F') { x_to = 6;}
    if (x_t == 'G') { x_to = 7;}
    if (x_t == 'H') { x_to = 8;}
    return {x_from, y_from, x_to, y_to};
}

bool human_strategy_t::possible_attack(const field_t &fld,
                                       std::set<std::pair<int, int>> &attack_checker) {
    attack_checker.clear();
    bool changed = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (player_num == 0) {
                if (fld.fld[i][j] == 'w' || fld.fld[i][j] == 'W') {
                    if (attacked_checkers(fld, attack_checker, std::pair<size_t, size_t>(j, i))) {
                        changed = true;
                    }
                }
            } else {
                if (fld.fld[i][j] == 'b' || fld.fld[i][j] == 'B') {
                    if (attacked_checkers(fld, attack_checker, std::pair<size_t, size_t>(j, i))) {
                        changed = true;
                    }
                }
            }
        }
    }

    return changed;
}

void human_strategy_t::on_incorrect_step(const step_t &step) const {
    std::cout << "Incorrect step:" << std::endl;
    char ch = '-';
    if (step.x_from == 1) {ch = 'A';}
    if (step.x_from == 2) {ch = 'B';}
    if (step.x_from == 3) {ch = 'C';}
    if (step.x_from == 4) {ch = 'D';}
    if (step.x_from == 5) {ch = 'E';}
    if (step.x_from == 6) {ch = 'F';}
    if (step.x_from == 7) {ch = 'G';}
    if (step.x_from == 8) {ch = 'H';}
    std::cout << "From x: " << ch << "; y: " << step.y_from << std::endl;
    if (step.x_to == 1) {ch = 'A';}
    if (step.x_to == 2) {ch = 'B';}
    if (step.x_to == 3) {ch = 'C';}
    if (step.x_to == 4) {ch = 'D';}
    if (step.x_to == 5) {ch = 'E';}
    if (step.x_to == 6) {ch = 'F';}
    if (step.x_to == 7) {ch = 'G';}
    if (step.x_to == 8) {ch = 'H';}
    std::cout << "To x: " << ch << "; y: " << step.y_to << std::endl;
}
