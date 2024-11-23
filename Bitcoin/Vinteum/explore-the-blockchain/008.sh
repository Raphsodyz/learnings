# Which public key signed input 0 in this tx:
#   `e5969add849689854ac7f28e45628b89f7454b83e9699e551ce14b6f90c86163`

#!/bin/zsh
tx="e5969add849689854ac7f28e45628b89f7454b83e9699e551ce14b6f90c86163"
transactions=$(bitcoin-cli getrawtransaction $tx)
decodedTransactions=$(bitcoin-cli decoderawtransaction $transactions)

if echo $decodedTransactions | jq -e '.vin[0].txinwitness' > /dev/null; then
    witness=$(echo $decodedTransactions | jq -r '.vin[0].txinwitness[2]')
    echo ${witness:4:66} #bash-like pk extract style for CI pipeline.
fi