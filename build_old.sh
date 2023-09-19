
if [ "" == "$1" ]; then
    echo "Not enough arguments"
    echo "bash build.sh  <number of nodes>, <maxPeers*>"
    exit 1
fi
bash clean.sh
go install -v ./...
bash clean.sh
bash genNodeCaps.sh $1
bash bootnode.sh

for i in $(seq 1 $(($1))); do
    tmp=$(( $i % 2 ))
    if [ $tmp -eq 0 ]; then
        echo "The node is validator"
        bash poa-net.sh $i true
    else
        echo "The node is not a validator"
        bash poa-net.sh $i false 
    fi
done


if [ "" = "$2" ]; then
    echo "No MaxPeer limit, starting $1 nodes" 
    for i in $(seq 1 $(($1))); do
        echo "nohup bash start.sh $i 1>logs/$i.log 2>&1 &"
        nohup bash start.sh $i 1>logs/$i.log 2>&1 &
    done
else
    echo "MaxPeer for each node is $2, starting $1 nodes"
    for i in $(seq 1 $(($1))); do
        echo "nohup bash start.sh $i $2 1>logs/$i.log 2>&1 &"
        nohup bash start.sh $i $2 1>logs/$i.log 2>&1 &
    done
fi
