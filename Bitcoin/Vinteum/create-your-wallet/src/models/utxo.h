#ifndef UTXO_H
#define UTXO_H

#include <string>

class Utxo{
public:
    std::string Txid;
    std::string Script;
    int VoutIndex;
    double Value;

    Utxo();
    Utxo(const std::string& txid, const std::string& script, int index, double value);
};

#endif