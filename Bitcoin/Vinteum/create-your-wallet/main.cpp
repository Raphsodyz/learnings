#include <cassert>
#include <exception>
#include <iostream>
#include <ostream>
#include "src/services/balance/walletstatebalance.h"

int main() {
  try {
    GetBalance();
    return 0;
    
  } catch (const std::exception &ex) {
    std::cout << ex.what() << std::endl;
  }
}