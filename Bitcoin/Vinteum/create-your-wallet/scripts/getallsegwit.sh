#!/bin/zsh

bitcoin-cli deriveaddresses "wpkh(tprv8ZgxMBicQKsPe2CnCcccz4hvAKAZPemK3XFM5cV2nWrv9gD7Z6M2knqa1zBKmeHtATpAMUSWrv9Yvk5mjSvFMwBNPzxr2yaFAko6V6JAQCd/84h/1h/0h/0/*)#f93a6ngu" "[0, 1999]" |
jq -r '.[]' | while read -r bech; do
  scriptPubkey=$(bitcoin-cli validateaddress "$bech" | jq -r '.scriptPubKey')
  echo "$scriptPubkey"
done