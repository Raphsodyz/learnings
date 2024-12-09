#include "walletrepository.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

WalletRepository::WalletRepository(const std::string& databaseName)
    : db(nullptr), dbName(databaseName) { }

bool WalletRepository::OpenConnection(const std::string databaseName){
    if (sqlite3_open(dbName.c_str(), &db) == SQLITE_OK) {
        return true;
    }
    else {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        return false;
    }
}

bool WalletRepository::CheckUtxoExists(const std::string& txid){
    if(db == nullptr){
        std::cerr << "Database is not open.\n";
        return false;
    }

    sqlite3_stmt* stmt;
    std::ostringstream oss;
    oss << "SELECT EXISTS(SELECT 1 FROM UTXO WHERE TXID = " << "'" << txid << "'" << ");";
    std::string query = oss.str();
    
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    
    rc = sqlite3_step(stmt);
    bool exists = false;

    if (rc == SQLITE_ROW) {
        int result = sqlite3_column_int(stmt, 0);
        exists = (result == 1);
    }

    sqlite3_finalize(stmt);
    return exists;
}

bool WalletRepository::InsertUtxo(const Utxo& utxo){
    if(db == nullptr){
        std::cerr << "Database is not open.\n";
        return false;
    }

    char* errorMessage = nullptr;
    std::ostringstream oss;

    oss << "INSERT INTO UTXO (TXID, VOUT_IDX, SCRIPT, VALUE) VALUES (" \
    << "'" << utxo.Txid << "'" \
    << ", '" << utxo.VoutIndex << "'" \
    << ", '" << utxo.Script << "'" \
    << ", '" << utxo.Value << "'" \
    << ");";

    std::string query = oss.str();
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "Error executing query: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
        return false;
    }
    
    return true;
}

WalletRepository::~WalletRepository(){
    CloseConnection();
}

void WalletRepository::CloseConnection() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}