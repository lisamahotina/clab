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
//    std::cout << "  1 2 3 4 5 6 7 8" << std::endl;
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
    x_from = (int)x_fr - 64;

    std::cout << "Type coordinates of the move: " << std::endl;
    int x_to = -1, y_to;
    char x_t;
    std::cin >> x_t >> y_to;
    x_to = (int)x_t - 64;

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
    x_to = (int)x_t - 64;
    return {x_from, y_from, x_to, y_to};
}

bool human_strategy_t::possible_attack(const field_t &fld,
                                       std::set<std::pair<int, int>> &attack_checker) {
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

void human_strategy_t::on_incorrect_step(const step_t &step) const {
    std::cout << "Incorrect step:" << std::endl;
    char ch = '-';
    ch = (char)(step.x_from + 64);
    std::cout << "From x: " << ch << "; y: " << step.y_from << std::endl;
    ch = (char)(step.x_to + 64);
    std::cout << "To x: " << ch << "; y: " << step.y_to << std::endl;
}
