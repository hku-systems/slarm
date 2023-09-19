#!/bin/bash
nodes=($(ls ../../../../poa-net | grep node | wc -l))

# for i in $(seq 1 $nodes);
# do
#     echo "'{"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc"
#     echo '{"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc &
# done

for i in $(seq 1 $nodes);
do
    while read j
    do 
        # echo $j | awk -F , '{print $3}' | awk -F : '{print $2}' | awk -F \" '{print $2}' >> peerCount.log
        echo "$i $j"
        break
    done < <(echo '{"jsonrpc":"2.0","method":"net_peerCount","params":[],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc)
done