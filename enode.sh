#!/bin/bash

echo "##############################################"
echo "Obtain enodes"
echo "##############################################"

nodes=($(ls ../../../../poa-net | grep node | wc -l))

# for i in $(seq 1 $nodes);
# do
#     echo "'{"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc"
#     echo '{"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc &
# done

rm enodes.txt
for i in $(seq 1 $nodes); do
    while read j
    do 
        echo $j | awk -F,  '{print $5}' | awk -F\" '{print $4}' >> enodes.txt
        break
    done < <(echo '{"jsonrpc":"2.0","method":"admin_nodeInfo","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc)
done