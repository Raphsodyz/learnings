# Only one single output remains unspent from block 123,321. What address was it sent to?

#!/bin/zsh
blockTransactions=$(bitcoin-cli getblock $(bitcoin-cli getblockhash 123321) | jq -r '.tx[]')
for transaction in $blockTransactions; do
    inputs=$(bitcoin-cli getrawtransaction $transaction true)
    voutsNumber=$(echo $inputs | jq '.vout | length')
    for i in $(seq 0 $((voutsNumber - 1))); do
        utxo=$(bitcoin-cli gettxout $transaction $i)
        if [ ! -z "$utxo" ]; then
            ADDRESS=$(echo $inputs | jq -r ".vout[$i].scriptPubKey.address")
            echo $ADDRESS
            exit 0
        fi
    done
done