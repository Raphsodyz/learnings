#ifndef DESCRIPTORDATA_H
#define DESCRIPTORDATA_H

#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept> 

class DescriptorData {
public:
    std::vector<uint8_t> PrivateKey;
    std::vector<uint8_t> ChainCode;
    std::vector<std::string> CompressedPublicKeys;
    std::vector<std::string> P2wpkhFromChildKeys;

    DescriptorData(const std::string &tprv);
    void DisplayMasterKeyChainCode() const;
    void DisplayChildCompressedPublicKeys() const;
    void DisplayChildP2wpkh() const;
};

#endif