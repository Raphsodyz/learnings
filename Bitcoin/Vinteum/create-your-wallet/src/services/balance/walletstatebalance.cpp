#include "walletstatebalance.h"
#include "../../cli/popen.h"
#include "../../models/descriptordata.h"
#include "../../models/utxo.h"
#include "../../repository/walletrepository.h"
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <memory>
#include <ostream>
#include <string>

static const std::string WALLET_NAME = "wallet_053";
static const std::string EXTENDED_PRIVATE_KEY =
    "tprv8ZgxMBicQKsPe2CnCcccz4hvAKAZPemK3XFM5cV2nWrv9gD7Z6M2knqa1zBKmeHtATpAMU"
    "SWrv9Yvk5mjSvFMwBNPzxr2yaFAko6V6JAQCd";
static const int LAST_TRANSACTION_BLOCK = 310;
static const std::string DATABASE_FILE_PATH = "src/database/wallet.db";

static std::string RemoveNullBackSpaceTerminator(std::string &variable) {
  if (variable.back() == '\0' || variable.back() == '\n') {
    variable.pop_back();
    return variable;
  }

  return variable;
}

static void SaveUtxos(std::map<std::string, Utxo>* utxoSet){
  WalletRepository walletRepository(DATABASE_FILE_PATH);
  for (const auto& utxo : *utxoSet) {
    if(walletRepository.OpenConnection("UTXO")){
      if(walletRepository.CheckUtxoExists(utxo.second.Txid)){
        continue;
      }

      walletRepository.InsertUtxo(utxo.second);
    }
  }
}

void GetBalance() {
  std::shared_ptr<DescriptorData> tprvData = std::make_shared<DescriptorData>(EXTENDED_PRIVATE_KEY);
  std::map<std::string, Utxo> utxoSet;
  long double balance = 0.0;

  for (int i = 1; i < LAST_TRANSACTION_BLOCK; i++) {
    std::string blockHash = BitcoinCli("getblockhash " + std::to_string(i));
    if (blockHash.empty()) {
      std::cout << "Attention! Is not found the hash of the block: " << i
                << std::endl;
      continue;
    }

    std::string block = BitcoinCli(
        "getblock " + RemoveNullBackSpaceTerminator(blockHash) + " 2");
    if (block.empty()) {
      std::cout << "Attention! Not found received values in transaction "
                   "outpoints in the blockhash: "
                << blockHash << std::endl;
      continue;
    }

    Json::Reader reader;
    Json::Value blockData;

    if (!reader.parse(RemoveNullBackSpaceTerminator(block), blockData)) {
      std::cerr << "Error parsing block JSON of hash: " << blockHash
                << std::endl;
      continue;
    }

    for (const auto &tx : blockData["tx"]) {
      for (const auto &output : tx["vout"]) {
        std::string hex = output["scriptPubKey"]["hex"].asString();
        for (const std::string &p2wpkh : tprvData->P2wpkhFromChildKeys) {
          if (p2wpkh == hex) {
            std::ostringstream key;
            key << tx["txid"].asString() << "-" << output["n"].asInt();

            utxoSet[key.str()] =
                Utxo(tx["txid"].asString(), hex, output["n"].asInt(),
                     output["value"].asDouble());
          }
        }
      }

      for (const auto &input : tx["vin"]) {
        if (input.isMember("txinwitness")) {
          const Json::Value &witness = input["txinwitness"];
          for (const auto &pubKey : tprvData->CompressedPublicKeys) {
            if (witness.size() > 1 && witness[1].asString() == pubKey) {
              std::ostringstream key;
              key << input["txid"].asString() << "-" << input["vout"].asInt();

              if (utxoSet.find(key.str()) != utxoSet.end()) {
                utxoSet.erase(key.str());
              }
            }
          }
        }
      }
    }
  }

  for (const auto &utxo : utxoSet) {
    balance += utxo.second.Value;
  }

  SaveUtxos(&utxoSet);
  std::cout << WALLET_NAME << " " << std::fixed << std::setprecision(8) << balance;
}