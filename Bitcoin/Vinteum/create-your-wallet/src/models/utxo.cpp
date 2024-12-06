#include "utxo.h"

Utxo::Utxo() : Txid(""), VoutIndex(0), Value(0.0) { }

Utxo::Utxo(const std::string& txid, int index, double value) 
    : Txid(txid), VoutIndex(index), Value(value) { }