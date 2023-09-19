package main

// #cgo pkgconfig: dl sgx_urts

import (
	"fmt"

	"github.com/ethereum/go-ethereum/core/enclave"
	"github.com/ethereum/go-ethereum/core/types"
)

func main() {
	fmt.Println("???")
	enclave.Test()
	e := enclave.NewEnclave()
	for i := 0; i < 100; i++ {
		tx := new(types.Transaction)
		tx.SetUrgent(true)
		tx.SetUrgentNonce((uint64)(i * 2))
		gap := e.Db.Add(tx)
		fmt.Println(gap)
	}

	for i := 0; i < 100; i++ {
		tx := new(types.Transaction)
		tx.SetUrgent(true)
		tx.SetUrgentNonce((uint64)(i*2 + 1))
		gap := e.Db.Add(tx)
		fmt.Println(gap)
	}

	// for i := 99; i >= 1; i-- {
	// 	tx := new(types.Transaction)
	// 	tx.SetUrgent(true)
	// 	tx.SetUrgentNonce((uint64)(i*2 + 1))
	// 	gap := e.Db.Add(tx)
	// 	fmt.Println(gap)
	// }
	for i := 0; i < 100; i++ {
		txn, _ := e.Db.GetTx(true, (uint64)(i))
		fmt.Println(txn.GetUrgentNonce())
	}

	e.DestroyEnclave()
}
