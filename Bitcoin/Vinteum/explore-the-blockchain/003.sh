# How many new outputs were created by block 123,456?

#!/bin/zsh
blockhash=$(bitcoin-cli getblockhash 123456)
blockdata=$(bitcoin-cli getblock $blockhash 2)

echo $blockdata | jq '[.tx[].vout | length] | add' 
