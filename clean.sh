# Created by Tianxiang 22/Feb/2020
# clean bootnode log, geth database and all datadir

# !/bin/bash

#echo "clean genesis json"

#sed -i '20s/.$//' ./genesis-clique-poa.json
pkill -f geth

yes | rm logs/*

clean_db="$GOPATH/bin/geth --nousb removedb"

echo $clean_db

yes | bash -c "$clean_db"

clean_bootnode="rm -rf ./bootnode.key ./bootnode.log"

echo $clean_bootnode

yes | bash -c "$clean_bootnode"

ps x | grep bootnode | awk '{print $1}'| sed -n '1p' > ./pid_1.txt

pid_1=$(cat ./pid_1.txt)


killer_1="kill -9 $pid_1"

echo $killer_1

bash -c "$killer_1"

ps x | grep bootnode | awk '{print $1}'| sed -n '2p' > ./pid_2.txt

pid_2=$(cat ./pid_2.txt)

killer_2="kill -9 $pid_2"

echo $killer_2

bash -c "$killer_2"

yes | rm -rf ./pid_*.txt

clean_datadir="rm -rf $GOPATH/poa-net"

echo $clean_datadir

yes | bash -c "$clean_datadir"

echo "clean genesis file"

linenum=`cat $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json | wc -l`  

linenum_last3=`expr $linenum - 2` 

echo $linenum_last3
if [ $linenum_last3 -ge "20" ];
then
   sed -i '20,'$linenum_last3'd' $GOPATH/src/github.com/ethereum/go-ethereum/genesis-clique-poa.json
fi

echo "Successfully cleaned all!!!"
