# How to Run SLARM

## Build and Install

### GO Ethereum 
Install go1.13.6.linux-amd64 and set the **$GOPATH**. Test by running 

```shell
go version
```

### Clone project
Must have the working copy under **$GOPATH/src/github.com/ethereum/go-ethereum**

Clone by running 
```shell
git clone git@github.com:osdi20p264/SLARM.git $GOPATH/src/github.com/ethereum/go-ethereum
```

### Build Executables
Switch to the go-ethereum repository root directory. Build all code by 

```shell
go install -v ./...
```

Executables are under **$GOPATH/bin**

### Test 

Test by running **geth --help** or **geth -h**

## Customize a private network

Run a cluster with **`poa-net.sh`** and **`start.sh`**

Example: To build a poa netwok with 2 validators (node1 and node2) and 1 normal node (node3), you should run by entering

```shell
bash clean.sh

bash bootnode.sh

bash poa-net.sh 1 true ; bash poa-net.sh 2 true ; bash poa-net.sh 3 false

bash start.sh 1

bash start.sh 2

bash start.sh 3

...
```

Might need wait for a short time for full syncnization

For next run, you should clean all by 

```shell
bash clean.sh
```

## Management API

Use *attach* to operate and see txn/block flow

Example, to manage node 1:

```shell
$GOPATH/bin/geth attach $GOPATH/poa-net/node1/geth.ipc
```

Connect to node 2 and submit a new transaction:

```shell
eth.sendTransaction({from:eth.coinbase, to:eth.accounts[0], value: web3.toWei(0.05, "ether")})
```

References:

1. [https://geth.ethereum.org/getting-started/dev-mode](https://geth.ethereum.org/getting-started/dev-mode)
2. [https://github.com/ethereum/go-ethereum/wiki/Management-APIs](https://github.com/ethereum/go-ethereum/wiki/Management-APIs)