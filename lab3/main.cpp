#include <iostream>
#include "int_list.h"

int main() {
    int_list list_1;
    for (int i = 0; i < 10; ++i) {
        list_1.push_front(i);
    }
    std::cout << list_1 << std::endl;

    int_list list_2 = list_1;
    std::cout << list_2.empty() << std::endl;

    list_2 = list_1.splice(2, 4);
    std::cout << list_2 << std::endl;

    return 0;kfkf,f 23
}
