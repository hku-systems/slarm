# !/bin/bash

echo "##############################################"
echo "Generating capacity of nodes"
echo "##############################################"

MaxPeer=50

if [ "" == "$1" ]; then
    echo "Usage: bash genNodeCaps.sh <Node Number>"
    exit 1
fi
NodesNum=$1

hostNum=$(cat hosts.txt | wc -l)
nodesPerHost=$(( $NodesNum / $hostNum ))
echo "hostNum: $hostNum, nodesPerHost: $nodesPerHost, Total number of nodes:  $(($hostNum * $nodesPerHost))"

# If not specified, the num of high cap nodes is nodes * 0.1
if [ "" == "$2" ]; then 
    HighCapNodesNum=$(awk "BEGIN {printf \"%d\", $NodesNum*0.1}")
else 
    HighCapNodesNum=$2
fi


lowCapVal=$(awk "BEGIN {printf \"%d\", sqrt($1)}")
highCapVal=$(($1-1))
if [ $highCapVal -gt $MaxPeer ]; then
    highCapVal=$MaxPeer
fi

echo "HighCap Nodes:$highCapVal, LowCap Nodes:$lowCapVal"

rm nodeCaps.csv
for host in $(cat hosts.txt); do
    port=30311
    for j in $(seq 1 $nodesPerHost); do
        if [ $HighCapNodesNum -gt 0 ]; then
            # high cap node
            echo "$host:$(( $port + j )),$highCapVal"
            echo "$host:$(( $port + j )),$highCapVal" >> nodeCaps.csv
            HighCapNodesNum=$(($HighCapNodesNum - 1))
        else 
            # low cap node
            echo "$host:$(( $port + j )),$lowCapVal"
            echo "$host:$(( $port + j )),$lowCapVal" >> nodeCaps.csv
        fi
    done 
    echo -e "\r"
done
