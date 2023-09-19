# created by Tianxiang 16/Jan/2020

# !/bin/bash
# bash cluster <number_of_nodes> <ip_addr> [[params]...]

num=$1
shift
ip_addr=$1
# default use network id 2020, may add to param later
networkid=2020
dir=$GOPATH/$networkid
echo $dir
echo $ip_addr
# initilize datadir
mkdir -p $dir/test-genesis
echo "using default genesis block -> initilized nodes' datadir"
echo "geth --nousb --datadir $dir/test-genesis init \
        $GOPATH/src/github.com/ethereum/go-ethereum/genesis-test.json "
geth --nousb --datadir $dir/test-genesis init \
        $GOPATH/src/github.com/ethereum/go-ethereum/genesis-test.json 

echo "create node datadir"


if [ ! -f "$dir/nodes" ]; then

    echo "[" >> $dir/nodes
    echo "in if"

    for ((i=0;i<$num;++i)); do
        id=`printf "%02d" $i`
        if [ ! $ip_addr = "" ]; 
        then
            echo "ip not null"
        else
            ip_addr="[::]"
            echo $ip_addr
        fi
        echo "create node $id"
        geth --nousb --identity "MyTestNode1" --nodiscover --networkid $networkid \
                --port 303$id --rpcport 81$id --datadir $GOPATH/$networkid/node/$id  \
                    init $GOPATH/src/github.com/ethereum/go-ethereum/genesis-test.json 
    
        echo "getting enode for instance $id ($i/$num)"
        eth="geth --nousb --identity "MyTestNode$id" --nodiscover --networkid $networkid \
            --port 303$id --rpcport 81$id --datadir $GOPATH/$networkid/node/$id console"
        # echo $eth
        cmd="$eth js <(echo 'console.log(admin.nodeInfo.enode); exit();') "
        echo $cmd

        # bash -c "$cmd" 

        gnome-terminal "run$id" -x bash -c "$cmd 2>$GOPATH/$networkid/node/$id/output.txt" 
        sleep 1
        # killall geth
        node_addr="enode://$(grep self $GOPATH/$networkid/node/$id/output.txt | awk -F '://' '{print $2}')"
        # echo $node_addr
        echo $node_addr >> $dir/nodes
        if ((i<num-1)); then
            echo "," >> $dir/nodes
        fi
    done

    echo "done"
    echo "]" >> $dir/nodes

fi

# echo $dir
mkdir -p $dir/data
mkdir -p $dir/log


# for ((i=0;i<N;++i)); do
#   id=`printf "%02d" $i`
#   # echo "copy $dir/data/$id/static-nodes.json"
#   mkdir -p $dir/data/$id
#   # cp $dir/nodes $dir/data/$id/static-nodes.json
#   echo "launching node $i/$N ---> tail-f $dir/log/$id.log"
#   echo GETH=$GETH bash ./gethup.sh $dir $id --networkid $network_id $*
#   GETH=$GETH bash ./gethup.sh $dir $id --networkid $network_id $*
# done