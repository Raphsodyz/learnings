#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include "popen.h"

std::string BitcoinCli(const std::string &command){
    std::string result;

    std::ostringstream oss;
    oss << "bitcoin-cli -signet " << command << std::endl;
    
    std::string fullCommand = oss.str();
    std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(fullCommand.c_str(), "r"), pclose);
    if(!pipe){
        throw std::runtime_error("Failed to open terminal with popen().");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}