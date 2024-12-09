# Create a 1-of-4 P2SH multisig address from the public keys in the four inputs of this tx:
#   `37d966a263350fe747f1c606b159987545844a493dd38d84b070027a895c4517`

#!/bin/zsh
tx="37d966a263350fe747f1c606b159987545844a493dd38d84b070027a895c4517"
transactionInputs=$(bitcoin-cli getrawtransaction $tx true | jq -r '.vin[].txinwitness[1]')

publicKey1=$(echo "$transactionInputs" | sed -n '1p' | sed 's/"//g')
publicKey2=$(echo "$transactionInputs" | sed -n '2p' | sed 's/"//g')
publicKey3=$(echo "$transactionInputs" | sed -n '3p' | sed 's/"//g')
publicKey4=$(echo "$transactionInputs" | sed -n '4p' | sed 's/"//g')

multisig_address=$(bitcoin-cli createmultisig 1 "[\"${publicKey1}\", \"${publicKey2}\", \"${publicKey3}\", \"${publicKey4}\"]")
echo $multisig_address | jq -r '.address'