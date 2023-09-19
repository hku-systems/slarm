# !/bin/bash

# bash RandomConnect.sh <maxpeer>

if [ "" == "$1" ]; then 
    MaxPeer=50
else
    MaxPeer="$1"
fi

echo "Maximum number of Peer is $MaxPeer"

bash peerCount.sh

# obtain enodes 
bash enode.sh

sleep 1

# Add Peers
nodes=($(ls ../../../../poa-net | grep node | wc -l))
for i in $(seq 1 $nodes); do
    for enode in $(shuf -n $MaxPeer enodes.txt ); do
        echo "i: $i enode: $enode"
        while read j; do 
            echo $j 
            break
        done < <( echo '{"jsonrpc":"2.0","method":"admin_addPeer","params":['\"$enode\"'],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc)
    done
done

sleep 1
bash peerCount.sh

# for i in $(seq 1 $nodes); do
#     for enode in $(cat enodes.txt); do
#         echo "i: $i enode: $enode"
#         echo  "echo {\"jsonrpc\":\"2.0\",\"method\":\"admin_addTrustedPeer\",\"params\":[\"$enode\"],\"id\":74} | sudo nc -U ../../../../poa-net/node$i/geth.ipc"
#     done
# done