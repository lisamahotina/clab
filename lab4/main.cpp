#include <memory>

#include "game/game.h"
#include "strategies/human.h"
#include "strategies/computer1.h"
#include "strategies/computer2.h"


int main() {
    auto player1 = std::make_shared<computer_strategy_t_1>("First player");
    auto player2 = std::make_shared<computer_strategy_t_2>("Second player");


//    game_t game(player1, player2);
//    game.play();

  for (int i = 0; i < 50; ++i) {
    game_t game(player1, player2);
    game.play();
  }

  player1->print_stat();
  player2->print_stat();


    return 0;
}
