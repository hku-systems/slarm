#!/bin/bash
source $HOME/.bashrc

if [ ! -f bootnode.key ];then
   $GOPATH/bin/bootnode -genkey bootnode.key
fi
killall $GOPATH/bin/bootnode
nohup $GOPATH/bin/bootnode -nodekey=bootnode.key -addr :30311 1>bootnode.log 2>&1 &





# op=$(grep enode bootnode.log | awk -F '://' '{print $2}')
