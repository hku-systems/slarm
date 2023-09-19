# Created by Tianxiang 08/Mar/2020

# !/bin/bash
# bash start.sh <totalnumber> [[params]...]

totalnumber=$1
ip=127.0.0.1

echo $totalnumber

# init datadir
let num=$totalnumber+1
dir=$GOPATH/poa-net/node$totalnumber

echo "$GOPATH/bin/geth --nousb --identity \"testnode$totalnumber\" --networkid 961213 --port `expr 30310 + $num` --rpcport `expr 8110 + $num` --datadir $dir init $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json"

$GOPATH/bin/geth --nousb --identity "testnode$totalnumber" --networkid 961213 --port `expr 30310 + $num` --rpcport `expr 8110 + $num` --datadir $dir init $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json 

# execute
# TODO: --rpcapi --gasprice
bootnode_addr=enode://"$(grep enode bootnode.log|tail -n 1|awk -F '://' '{print $2}'|awk -F '@' '{print $1}')""@$ip:0?discport=30311"
raw_signer_addr="$(grep Public $dir/signer-log$totalnumber | awk -F'0x' -vOFS="0x" '{$1="";$1=$1}1')"

echo "$GOPATH/bin/geth --nodiscover --verbosity 4 --nousb --syncmode "full" --identity "testnode$totalnumber" --networkid 961213 --port `expr 30310 + $num` --rpcaddr $ip --rpcport `expr 8110 + $num` --datadir $dir --bootnodes "$bootnode_addr" --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine" 
$GOPATH/bin/geth --nodiscover --verbosity 4 --nousb --syncmode "full" --identity "testnode$totalnumber" --networkid 961213 --port `expr 30310 + $num` --rpcaddr $ip --rpcport `expr 8110 + $num` --datadir $dir  --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine 


# else
#     echo "$GOPATH/bin/geth --maxpeers $2 --verbosity 5 --nousb --syncmode "full" --identity "testnode$totalnumber" --networkid 961213 --port `expr 30310 + $num` --rpcport `expr 8110 + $num`  --datadir $dir --bootnodes "$bootnode_addr" --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine"
#     $GOPATH/bin/geth  --nodiscover  --maxpeers $2 --verbosity 5 --nousb --syncmode "full" --identity "testnode$totalnumber" --networkid 961213 --port `expr 30310 + $num` --rpcport `expr 8110 + $num` --datadir $dir --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine 
# fi



