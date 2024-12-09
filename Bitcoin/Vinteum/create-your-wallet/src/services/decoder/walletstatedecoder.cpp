#include "walletstatedecoder.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <ostream>
#include <secp256k1.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static const char *base58Chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
const char* SECP256K1_ORDER_HEX = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
const char* G_HEX = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";

static std::vector<uint32_t> Bip32PathParser(const std::string &path){
  std::vector<uint32_t> indices;
  std::istringstream ss(path);
  std::string level;

  while (std::getline(ss, level, '/')) {
    if (level.empty()) continue;

    bool hardened = (level.back() == 'h' || level.back() == '\'');
    if (hardened) level.pop_back();

    uint32_t index = std::stoi(level);
    if (hardened) index += 0x80000000;

    indices.push_back(index);
  }
  
  return indices;
}

static std::vector<uint8_t> DeriveCompressedPublicKey(const std::vector<uint8_t> &parentKey) {
  secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

  secp256k1_pubkey pubkey;
  unsigned char* parentKeyAsChar = new unsigned char[parentKey.size()];
  std::memcpy(parentKeyAsChar, parentKey.data(), parentKey.size());

  if (!secp256k1_ec_seckey_verify(ctx, parentKeyAsChar)) {
    secp256k1_context_destroy(ctx);
    delete [] parentKeyAsChar;
    throw std::runtime_error("The parent key is not valid.");
  }

  delete [] parentKeyAsChar;
  if (!secp256k1_ec_pubkey_create(ctx, &pubkey, parentKey.data())) {
    secp256k1_context_destroy(ctx);
    throw std::runtime_error("Failed to create public key.");
  }

  std::vector<uint8_t> compressedPubKey(33);
  size_t outputLength = compressedPubKey.size();

  if(secp256k1_ec_pubkey_serialize(ctx, compressedPubKey.data(), &outputLength, &pubkey, SECP256K1_EC_COMPRESSED) != 1){
    secp256k1_context_destroy(ctx);
    throw std::runtime_error("Failed to serialize public key");
  };

  secp256k1_context_destroy(ctx);
  return compressedPubKey;
}

BIGNUM* Parse256(const std::vector<uint8_t>& data) {
  BIGNUM* bn = BN_new();
  BN_bin2bn(data.data(), data.size(), bn);
  return bn;
}

std::vector<uint8_t> ComputeChildPrivateKey(const std::vector<uint8_t>& IL, const std::vector<uint8_t>& parentPrivateKey){
  BIGNUM* bn_IL = Parse256(IL);
  BIGNUM* bn_kpar = Parse256(parentPrivateKey);
  BIGNUM* bn_n = nullptr;
  BN_hex2bn(&bn_n, SECP256K1_ORDER_HEX);

  BN_add(bn_IL, bn_IL, bn_kpar);
  BN_mod(bn_IL, bn_IL, bn_n, BN_CTX_new());

  std::vector<uint8_t> childKey(32);
  BN_bn2binpad(bn_IL, childKey.data(), 32);

  BN_free(bn_IL);
  BN_free(bn_kpar);
  BN_free(bn_n);

  return childKey;
}



static std::pair<std::vector<uint8_t>, std::vector<uint8_t>> DeriveChildKeyAndChaincode(
  const std::vector<uint8_t>& parentKey, const std::vector<uint8_t>& parentChainCode, uint32_t index) {
  
  std::vector<uint8_t> data;
  if(index >= 0x80000000){
    data.push_back(0x00);
    data.insert(data.end(), parentKey.begin(), parentKey.end());
  } else {
    std::vector<uint8_t> parentPubKey = DeriveCompressedPublicKey(parentKey);
    data.insert(data.end(), parentPubKey.begin(), parentPubKey.end());
  }

  data.push_back((index >> 24) & 0xFF);
  data.push_back((index >> 16) & 0xFF);
  data.push_back((index >> 8) & 0xFF);
  data.push_back(index & 0xFF);

  unsigned char I[64];
  HMAC(EVP_sha512(), parentChainCode.data(), parentChainCode.size(), data.data(), data.size(), I, nullptr);

  std::vector<uint8_t> IL(I, I + 32);
  std::vector<uint8_t> IR(I + 32, I + 64);

  std::vector<uint8_t> computedChildPrivateKey = ComputeChildPrivateKey(IL, parentKey);
  return {computedChildPrivateKey, IR};
}

static std::vector<uint8_t> DeriveKeyAtThePath(
  const std::vector<uint8_t>& masterKey, const std::vector<uint8_t>& masterChainCode, const std::string& path) {
  
  auto indexes = Bip32PathParser(path);
  auto currentKey = masterKey;
  auto currentChainCode = masterChainCode;

  for (uint32_t index : indexes) {
    auto result = DeriveChildKeyAndChaincode(currentKey, currentChainCode, index);
    currentKey = result.first;
    currentChainCode = result.second;
  }

  return currentKey;
}

static std::vector<uint8_t> DeriveP2WPKH(const std::vector<uint8_t>& compressedPubKey){
  unsigned char sha256Hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256Ctx;
  SHA256_Init(&sha256Ctx);
  SHA256_Update(&sha256Ctx, compressedPubKey.data(), compressedPubKey.size());
  SHA256_Final(sha256Hash, &sha256Ctx);

  unsigned char ripemd160Hash[RIPEMD160_DIGEST_LENGTH];
  RIPEMD160_CTX ripemd160Ctx;
  RIPEMD160_Init(&ripemd160Ctx);
  RIPEMD160_Update(&ripemd160Ctx, sha256Hash, SHA256_DIGEST_LENGTH);
  RIPEMD160_Final(ripemd160Hash, &ripemd160Ctx);

  std::vector<uint8_t> witnessProgram;

  witnessProgram.push_back(0x00); //version
  witnessProgram.push_back(0x14); //length
  witnessProgram.insert(witnessProgram.end(), ripemd160Hash, ripemd160Hash + RIPEMD160_DIGEST_LENGTH);

  return witnessProgram;
}

std::vector<uint8_t> Base58Decode(const std::string &tprv) {
  BIGNUM *bn = BN_new();
  BN_zero(bn);

  for(char base : tprv){
    const char *p = strchr(base58Chars, base);
    if(p){
      BN_mul_word(bn, 58);
      BN_add_word(bn, p - base58Chars);
    }
  }

  int requiredSize = BN_num_bytes(bn);
  std::vector<uint8_t> decoded(requiredSize);
  int size = BN_bn2bin(bn, decoded.data());
  BN_free(bn);

  return decoded;
}

std::string ByteVectorToString(const std::vector<uint8_t>& data) {
  std::ostringstream hexStream;
  for (auto byte : data) {
    hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }

  return hexStream.str();
}

std::vector<std::pair<std::string, std::string>> DeriveP2wpkhFromChildKeys(const std::vector<uint8_t> &masterKey, const std::vector<uint8_t> &masterChaincode, uint32_t numberOfKeys) {
  std::vector<std::pair<std::string, std::string>> result;
  for(uint32_t index = 0; index < numberOfKeys; index++){
    std::string descriptorPath = "84h/1h/0h/0/" + std::to_string(index);

    std::vector<uint8_t> privateKey = DeriveKeyAtThePath(masterKey, masterChaincode, descriptorPath);
    std::vector<uint8_t> compressedPubKey = DeriveCompressedPublicKey(privateKey);
    std::vector<uint8_t> p2wpkh = DeriveP2WPKH(compressedPubKey);

    result.push_back(std::make_pair(ByteVectorToString(compressedPubKey), ByteVectorToString(p2wpkh)));
  }

  return result;
}