totaltxn=$1
node=2

for (( i=0; i<$totaltxn; i++ ))
do      
    geth attach $GOPATH/poa-net/node$node/geth.ipc --exec "eth.sendTransaction({from:eth.coinbase, to:eth.accounts[0], nonce:$i})"
    echo "send new txn!"
    num=$(geth attach $GOPATH/poa-net/node$node/geth.ipc --exec "eth.blockNumber")
    echo -n "blockNumber:$num" >>block.log

    geth attach $GOPATH/poa-net/node$node/geth.ipc --exec "eth.getBlock($num)" >>block.log

    # sleep 1

done
