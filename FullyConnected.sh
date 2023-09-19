# !/bin/bash
nodes=($(ls ../../../../poa-net | grep node | wc -l))

bash peerCount.sh
bash enode.sh

for i in $(seq 1 $nodes); do
    for enode in $(cat enodes.txt); do
        echo "i: $i enode: $enode"
        while read j; do 
            echo $j 
            break
        done < <( echo '{"jsonrpc":"2.0","method":"admin_addPeer","params":['\"$enode\"'],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc)
    done
done

bash peerCount.sh