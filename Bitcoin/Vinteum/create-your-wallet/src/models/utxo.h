#ifndef UTXO_H
#define UTXO_H

#include <string>

class Utxo{
public:
    std::string Txid;
    int VoutIndex;
    double Value;

    Utxo();
    Utxo(const std::string& txid, int index, double value);
};

#endif