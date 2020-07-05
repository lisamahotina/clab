#pragma once

#include <string>

#include "../game/structures.h"
#include "../strategies/computer_interface.h"

class computer_strategy_t_2 : public computer_interface_t {
public:
    explicit computer_strategy_t_2(std::string name);

    bool possible_attack(const field_t &field, std::set<std::pair<int, int>> &attack_checker);

    step_t make_step(const field_t &field);

    step_t next_step(const field_t &field, std::pair<size_t, size_t> current_checker);

    void print_stat() const;

private:
    std::string name;
};