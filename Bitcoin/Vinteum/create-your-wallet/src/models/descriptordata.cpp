#include "descriptordata.h"
#include "../services/decoder/walletstatedecoder.h"
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

DescriptorData::DescriptorData(const std::string &extendedKey) {
    auto decodedBytes = Base58Decode(extendedKey);

    ChainCode.assign(decodedBytes.begin() + 13, decodedBytes.begin() + 45);
    PrivateKey.assign(decodedBytes.begin() + 46, decodedBytes.begin() + 78);

    std::vector<std::pair<std::string, std::string>> keys = DeriveP2wpkhFromChildKeys(PrivateKey, ChainCode, 2000);
    for (const auto& key : keys) {
        CompressedPublicKeys.push_back(key.first);
        P2wpkhFromChildKeys.push_back(key.second);
    }
}

void DescriptorData::DisplayMasterKeyChainCode() const {
    std::cout << "Master private key: ";
    for (auto b : PrivateKey) std::cout << std::hex << (int)b;
    std::cout << std::endl;

    std::cout << "Master chain code: ";
    for (auto b : ChainCode) std::cout << std::hex << (int)b;
    std::cout << std::endl;
}

void DescriptorData::DisplayChildP2wpkh() const{
    for(int i = 0; i < P2wpkhFromChildKeys.size(); i++){
        std::cout << "P2WPKH child key: ";
        for (auto b : P2wpkhFromChildKeys[i]) std::cout << std::hex << (int)b;
        std::cout << std::endl;
    }
}

void DescriptorData::DisplayChildCompressedPublicKeys() const {
    for(int i = 0; i < CompressedPublicKeys.size(); i++){
        std::cout << "Compressed public child key: ";
        for (auto b : CompressedPublicKeys[i]) std::cout << std::hex << (int)b;
        std::cout << std::endl;
    }
}