#include "../libs/sqlite/sqlite3.h"
#include "../models/utxo.h"

#ifndef WALLETREPOSITORY_H
#define WALLETREPOSITORY_H

class WalletRepository{
    private:
        sqlite3* db;
        std::string dbName;
        void CloseConnection();

    public:
        WalletRepository(const std::string& databaseName);
        ~WalletRepository();

        bool OpenConnection(const std::string databaseName);
        bool CheckUtxoExists(const std::string& txid);
        bool InsertUtxo(const Utxo& utxo);
};

#endif