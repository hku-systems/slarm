rm block.log

num=$(geth attach $GOPATH/poa-net/node1/geth.ipc --exec "eth.blockNumber")

echo -e "blockNumber:$num" >>block.log

for (( i=0; i<=$num; i++ ))
do      
    echo -n "blockNumber:$i" >>block.log
    geth attach $GOPATH/poa-net/node1/geth.ipc --exec "eth.getBlock($i)" >>block.log
    # sleep 1
done
