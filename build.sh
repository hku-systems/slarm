
if [ "" == "$1" ]; then
    echo "Not enough arguments"
    echo "bash build.sh  <number of nodes>"
    exit 1
fi

bash clean.sh
go install -v ./...
bash clean.sh
bash bootnode.sh

for i in $(seq 1 $(($1))); do
    if [ $i -eq 1 ]; then
        echo "The node is validator"
        bash poa-net.sh $i true
    else
        echo "The node is not a validator"
        bash poa-net.sh $i false 
    fi
done

# Start nodes
yes | rm -r logs/
mkdir logs
for i in $(seq 1 $(($1))); do
    # cap=$( echo $line | awk -F, '{print $2}' )
    # echo "nohup bash start.sh $i $cap 1>logs/$i.log 2>&1 &"
    nohup bash start.sh $i 1>logs/$i.log 2>&1 &
done


# AddPeers
sleep 1
bash genNodeCaps.sh $1
bash CapsConnect.sh
# bash FullyConnected.sh