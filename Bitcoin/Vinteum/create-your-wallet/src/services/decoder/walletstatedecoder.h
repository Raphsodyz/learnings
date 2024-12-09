#include <cstdint>
#include <string>
#include <vector>

#ifndef WALLETSTATEDECODER_H
#define WALLETSTATEDECODER_H

std::vector<uint8_t> Base58Decode(const std::string &tprv);
std::vector<std::pair<std::string, std::string>> DeriveP2wpkhFromChildKeys(const std::vector<uint8_t> &masterKey, const std::vector<uint8_t> &masterChaincode, uint32_t numberOfKeys);

#endif