#include "utxo.h"

Utxo::Utxo() : Txid(""), Script(""), VoutIndex(0), Value(0.0) { }

Utxo::Utxo(const std::string& txid, const std::string& script, int index, double value) 
    : Txid(txid), Script(script), VoutIndex(index), Value(value) { }