# !/bin/bash
# Usage: bash TableConnect.sh

echo "##############################################"
echo "TableConnect.sh Adding Peers"
echo "##############################################"

# obtain enodes 
bash enode.sh

sleep 1

# number of nodes
nodesNum=($(ls ../../../../poa-net | grep node | wc -l))

# creat peer record file for each node 
yes | rm -r temp
mkdir temp
for i in $(seq 1 $nodesNum); do
    touch temp/PeerRecord_$i
done

# Graphviz
rm topology.gv
echo "graph G {" >> topology.gv

for i in $(seq 1 $nodesNum); do
    myEnode=$(sed -n ${i}p enodes.txt)
    
    # cap of the node, read from nodeCaps.csv
    cap=$(sed -n ${i}p nodeCaps.csv | awk -F, '{print $2}')

    if [ $cap -eq $(($nodesNum-1)) ] || [ $cap -eq 50 ]; then
        echo "$i [style=\"filled\", fillcolor=\"red\"]" >> topology.gv
    fi

    # add peers for node i
    peersExist=$(cat temp/PeerRecord_$i | wc -l) 
    echo "Node$i already have $peersExist peers, need $(($cap-$peersExist)) peers."

    for j in $(seq $(($peersExist+1)) $cap); do
        # select a node with minimal peers (the more peers, the bigger the PeerRecord file is)
        sortedFiles=$(ls temp/* -Slr)
        index=1 # index in the $sortedFiles
        node=$(echo "$sortedFiles" | awk -F_ '{print $2}' | sed -n ${index}p) # remote node num
        enode=$(cat enodes.txt | sed -n ${node}p)
        # make sure it is not node_i or node_i's existing peer.
        while [ $(grep "$enode" "temp/PeerRecord_$i" | wc -l) -ne 0 ] || [ $node -eq $i ]; do
            echo "Select the next node"
            index=$(($index+1))
            node=$(echo "$sortedFiles" | awk -F_ '{print $2}' | sed -n ${index}p)
            enode=$(sed -n ${node}p enodes.txt)
        done

        # add remote enode to my record, add my enode to remote record
        echo "$enode" >> temp/PeerRecord_$i
        echo "$myEnode" >> temp/PeerRecord_$node

        # Graphviz
        echo "$i -- $node" >> topology.gv 

        # add peer via rpc
        while read k; do
            echo $k 
            break
        done < <( echo '{"jsonrpc":"2.0","method":"admin_addPeer","params":['\"$enode\"'],"id":74}' | sudo nc -U ../../../../poa-net/node$i/geth.ipc)
    done
done
# Graphviz
echo "}" >> topology.gv

# yes | rm -r temp

sleep 1

bash peerCount.sh

dot -Tpng topology.gv -o topology.png