# Which tx in block 257,343 spends the coinbase output of block 256,128?

#!/bin/zsh
coinbaseTxid=$(bitcoin-cli getblock $(bitcoin-cli getblockhash 256128) | jq -r '.tx[0]')
newBlockTransactions=$(bitcoin-cli getblock $(bitcoin-cli getblockhash 257343) | jq -r '.tx[]')

for txid in $newBlockTransactions; do
  inputs=$(bitcoin-cli getrawtransaction $txid true | jq -r '.vin[].txid')
  for input in $inputs; do
    if [ "$input" = "$coinbaseTxid" ]; then
      echo "$txid"
      exit 0
    fi
  done
done