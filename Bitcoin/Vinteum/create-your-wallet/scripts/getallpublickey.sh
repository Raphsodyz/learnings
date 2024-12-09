#!/bin/zsh

bitcoin-cli deriveaddresses "wpkh(tprv8ZgxMBicQKsPe2CnCcccz4hvAKAZPemK3XFM5cV2nWrv9gD7Z6M2knqa1zBKmeHtATpAMUSWrv9Yvk5mjSvFMwBNPzxr2yaFAko6V6JAQCd/84h/1h/0h/0/*)#f93a6ngu" "[0, 1999]" |
jq -r '.[]' | while read -r address; do
    publicKey=$(bitcoin-cli getaddressinfo $address | jq -r '.pubkey')
    echo $publicKey
done