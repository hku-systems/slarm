# Created by Tianxiang 20/Feb/2020

# !/bin/bash
# bash poa-net.sh <nodeid> <is_validator?> [[params]...]
# A 3-nodes cluster with node1 and node3 being the validator:
# bash poa-net.sh 1 true
# bash poa-net.sh 2 false
# bash poa-net.sh 3 true

id=$1
shift
is_validator=$1
echo "node$id is validator? $is_validator"

dir=$GOPATH/poa-net/node$id
echo "start node $id, initilized on path $dir"

if [ ! -f bootnode.log ];then
    echo -e "\033[40;31mError!!! please run bootnode.sh first \033[0m"
    exit
fi

ip=127.0.0.1

bootnode_addr=enode://"$(grep enode bootnode.log|tail -n 1|awk -F '://' '{print $2}'|awk -F '@' '{print $1}')""@$ip:0?discport=30301"

echo "bootnode enode: $bootnode_addr"

echo "listening on port 30301"


if [ ! -d "$dir" ]; 
then
    mkdir -p $dir
    echo "create datadir under $dir"
else
    echo -e "\033[40;31mError!!! $dir already exists! \033[0m"
    exit
fi

if [ ! -f "$GOPATH/poa-net/pwd" ];then
    echo "bccdn" >> $GOPATH/poa-net/pwd
    echo "password created"
fi 
   
# signer account and write to genesis
echo "$GOPATH/bin/geth --nousb account new --datadir $dir --password $GOPATH/poa-net/pwd 1>$dir/signer-log$id"

$GOPATH/bin/geth --nousb account new --datadir $dir --password $GOPATH/poa-net/pwd 1>$dir/signer-log$id

raw_signer_addr="$(grep Public $dir/signer-log$id | awk -F'0x' -vOFS="0x" '{$1="";$1=$1}1')"
echo $raw_signer_addr
echo ${#raw_signer_addr}

signer_addr=${raw_signer_addr:2}
echo $signer_addr
echo ${#signer_addr}

echo $signer_addr >> $GOPATH/poa-net/account_list
sort $GOPATH/poa-net/account_list -o $GOPATH/poa-net/account_list

# if current node is validator, put it into extradata

if [ $is_validator == "true" ];
then
    if [ ! -f "$GOPATH/poa-net/extradatastore" ];
    then
        echo -n $signer_addr >> $GOPATH/poa-net/extradatastore 
    else
        var=$(cat $GOPATH/poa-net/extradatastore)
        if [ $var  \<  $signer_addr ];
        then
            echo -n $signer_addr >> $GOPATH/poa-net/extradatastore 
        else
            new=$signer_addr$var
            echo -n $new > $GOPATH/poa-net/extradatastore 
        fi
        
    fi 

fi

v_addr=$(cat $GOPATH/poa-net/extradatastore)
sed -i '18c\  "extraData": "0x0000000000000000000000000000000000000000000000000000000000000000'$v_addr'0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",' ./genesis-clique-poa.json

let line=19

if [ $id == "1" ];
then    
    sed -ie ''$line'a\    "'$signer_addr'": { "balance": "0x200000000000000000000000000000000000000000000000000000000000000" }' ./genesis-clique-poa.json 
else
# first clean all alloc
    linenum=`cat $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json | wc -l`  

    linenum_last3=`expr $linenum - 2` 

    echo $linenum_last3
    if [ $linenum_last3 -ge "20" ];
    then
        sed -i '20,'$linenum_last3'd' $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json
    fi
# them assign pre-fund
    tail=$(cat $GOPATH/poa-net/account_list | wc -l )
    let temp=0
    while read addr
    do
        let temp=$temp+1
        if [ $temp -ne $tail ];
        then
            sed -ie ''$line'a\    "'$addr'": { "balance": "0x200000000000000000000000000000000000000000000000000000000000000" },' ./genesis-clique-poa.json 
            let line=$line+1
        else
            sed -ie ''$line'a\    "'$addr'": { "balance": "0x200000000000000000000000000000000000000000000000000000000000000" }' ./genesis-clique-poa.json 
        fi
    done < $GOPATH/poa-net/account_list

    # sed -i ''$line's/$/&,/' ./genesis-clique-poa.json
    # sed -ie ''$line'a\    "'$signer_addr'": { "balance": "90000000000000000000000000000000" }' ./genesis-clique-poa.json 
fi

# # init datadir
# let num=$id+1

# echo "$GOPATH/bin/geth --nousb --identity "testnode$id" --networkid 1234 --port 3030$num --rpcport 810$num --datadir $dir init $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json"

# $GOPATH/bin/geth --nousb --identity "testnode$id" --networkid 1234 --port 3030$num --rpcport 810$num --datadir $dir init $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json 

# # execute
# echo "$GOPATH/bin/geth --nousb --syncmode "full" --identity "testnode$id" --networkid 1234 --port 3030$num --rpcport 810$num --datadir $dir --bootnodes "$bootnode_addr" --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine"

# $GOPATH/bin/geth --nousb --syncmode "full" --identity "testnode$id" --networkid 1234 --port 3030$num --rpcport 810$num --datadir $dir --bootnodes "$bootnode_addr" --unlock $raw_signer_addr  --password $GOPATH/poa-net/pwd --mine 













